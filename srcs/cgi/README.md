# CGI

## 1.wtf q es eso de CGI

Nuestro servidor, hasta ahora, sabe hacer una cosa si le pides un archivo
(`/index.html`), lo busca en el disco, lo lee tal cual y te lo manda. Ese
archivo siempre dice lo mismo, la respuesta nunca cambia. A esto se le llama
contenido **estatico**.

El problema es que a veces queremos que el servidor **haga algo** en vez de
solo leer un archivo. Por ejemplo: "dime la hora actual", "procesa este
formulario", "consulta esto y calculalo". Eso no puede ser un archivo fijo,
tiene que **ejecutarse** cada vez que alguien lo pide, y puede dar una
respuesta distinta cada vez.

El **CGI (Common Gateway Interface)** es el acuerdo estandar mas antiguo y
mas simple que existe para que un servidor web pueda ejecutar un programa
externo (un script) y usar lo que ese programa imprime como respuesta HTTP.

La idea de fondo, resumida en una frase: **el script no describe la
respuesta, el script ES la respuesta**. Si el script hace `print("Hola")`,
eso es literalmente lo que le llega al navegador.

## 2. Que pasa cuando subes un `.py` al servidor y alguien lo pide

el camino completo de una peticion es el siguientee usando como
ejemplo `GET /cgi-bin/hello.py?nombre=Ismael`.

**Paso 1 — El servidor detecta que no es un archivo normal.**
Mira la extension del archivo pedido (`.py`) y, segun lo que diga la
configuracion, decide que esa extension no se sirve leyendo el archivo, sino
**ejecutandolo**.

**Paso 2 — El servidor se clona en dos.**
Aqui es donde entramos con nuestro querido `fork()`. Esta funcion hace una copia casi identica del
propio proceso del servidor: de repente hay dos procesos ejecutando el mismo
programa desde el mismo punto exacto. A partir de aqui, cada uno sigue su
camino:

- La copia **hija** va a convertirse en tu script `hello.py`.
- La copia **padre** es el servidor, que se queda vigilando.

**Paso 3 — Se montan dos "tuberias" antes de clonar.**
Antes del `fork()`, el servidor crea dos pipes

- Una para **mandarle datos al script** (sera su entrada estandar, `stdin`).
- Otra para **recibir lo que el script escribe** (sera su salida estandar,
  `stdout`).

**Paso 4 — La copia hija se convierte en el script.**
La hija conecta esas tuberias a su `stdin`/`stdout`, se cambia al directorio
donde vive el script (para que si el script usa rutas relativas, funcionen),
y llama a `execve()`. Esta funcion **sustituye** el proceso entero: deja de
ser una copia del servidor y pasa a ser literalmente el interprete de Python
ejecutando tu `hello.py`. Ya no hay marcha atras: o funciona, o el proceso
muere ahi mismo.

**Paso 5 — El servidor le pasa la peticion al script, pero de una forma muy
particular: variables de entorno.**
Este es el truco central de CGI. Cuando ejecutas cualquier programa, este
"hereda" un conjunto de variables de entorno (como `PATH` o `HOME`). El
estandar CGI aprovecha justo eso: antes de ejecutar el script, se rellenan
variables con nombres fijos que cualquier script CGI sabe leer:

| Variable          | Que significa                                   |
|-------------------|--------------------------------------------------|
| `REQUEST_METHOD`  | GET, POST, DELETE...                              |
| `QUERY_STRING`    | Lo que va despues del `?` en la URL               |
| `CONTENT_LENGTH`  | Cuantos bytes de body vienen (si hay)             |
| `CONTENT_TYPE`    | Tipo del body, si lo hay                          |
| `HTTP_XXX`        | Cada cabecera HTTP que mando el cliente           |

el script `hello.py` simplemente lee `os.environ["QUERY_STRING"]` y ya tiene
el `nombre=Ismael` de la URL, sin que el servidor le tenga que "hablar" por
ningun canal especial.

**Paso 6 — Si la peticion trae un body (por ejemplo, un formulario POST), se
lo mandamos por la tuberia de entrada.**
Un body puede pesar mucho para meterlo en una variable de entorno, asi que
en vez de eso se escribe en el `stdin` del script, como si se lo estuvieras
tecleando tu mismo al programa. El script lo lee de su entrada estandar,
igual que leerias texto tecleado por teclado.

**Paso 7 — El script hace su trabajo e imprime la respuesta por `stdout`.**
Todo lo que el script imprime (cabeceras tipo `Content-Type: ...`, una linea
en blanco, y luego el contenido) es justo lo que el servidor va a devolver
al navegador.

**Paso 8 — El servidor (proceso padre) recoge esa salida sin bloquearse.**
Aqui esta la parte mas delicada de todo el proyecto: el servidor **no puede
quedarse parado** esperando a que el script termine, porque mientras tanto
dejaria de atender al resto de clientes conectados. Por eso las dos tuberias
(entrada y salida del script) se meten dentro del mismo `poll()` unico que
ya vigila a todos los clientes normales. El servidor solo lee o escribe en
esas tuberias cuando el sistema le avisa "ya hay algo, puedes hacerlo sin
quedarte colgado". Cuando la tuberia de salida se cierra (el script ha
terminado y ha muerto), el servidor sabe que la respuesta ya esta completa
del todo.

**Paso 9 — El servidor separa esa salida en cabeceras + cuerpo, y construye
la respuesta HTTP real, que es lo que finalmente le llega al navegador.**

## 3. Los dos archivos del modulo, y que hace cada parte

- **`CgiHandler.hpp`**: el "menu" — que cosas puede hacer este modulo
  (arrancar el script, escribir su body, leer su salida, saber si ha
  terminado, matarlo si se pasa de tiempo...), sin entrar en como lo hace.
- **`CgiHandler.cpp`**: el "como". Dentro hay dos bloques bien diferenciados:
  1. **Lanzar el script como proceso aparte** (`fork` + pipes + `execve`):
     todo el Paso 2, 3 y 4 de arriba.
  2. **Vigilar esas tuberias sin bloquearse** (`poll` desde fuera + lectura
     y escritura no bloqueante): el Paso 8 de arriba.

## 4. Como se prueba (sin esperar a que este conectado al servidor)

Todavia no esta conectado a `Client`/`Server`, asi que hay un mini programa
de prueba aparte que hace exactamente lo que hara el servidor mas adelante:
lanza el script de verdad, monta el mismo tipo de vigilancia con `poll()`, y
comprueba que la respuesta es la esperada.

```sh
make test-cgi
```

Esto lanza 3 casos (GET y POST contra un script en Python, GET contra uno en
sh) y dice claramente cual pasa y cual no.

## 5. Que lenguajes soportamos y por que mezclar dos es un lio

De momento hemos probado con **Python** y con **shell (`sh`)**, a proposito,
para no atarnos a un solo lenguaje.

La complicacion real es esta: **quien ejecuta el script es el sistema
operativo, no nosotros**, y el sistema operativo necesita saber el camino
**exacto** al programa que lo va a ejecutar. No le puedes decir "ejecuta
esto con Python": tienes que decirle literalmente "ejecuta el binario que
esta en `/usr/bin/python3`, y pasale este script como argumento". Cada
lenguaje tiene su interprete en un sitio distinto del disco:

- Python -> `/usr/bin/python3`
- Shell  -> `/bin/sh`
- PHP    -> `/usr/bin/php-cgi` (o donde este instalado)

Por eso el archivo de configuracion va a tener que decir, para cada
extension de archivo, cual es el interprete que le corresponde (`.py` ->
tal ruta, `.php` -> tal otra), y el modulo simplemente usa la ruta que le
den. El modulo en si no sabe ni le importa que lenguaje es; solo ejecuta lo
que le digan.

Un detalle con el que ya nos comimos un bug probando esto: si le pasas al
sistema una ruta que en realidad es una frase con espacios (por ejemplo
`"/usr/bin/env python3"`), falla en silencio, porque el sistema no entiende
frases como comando, entiende **una ruta a un unico archivo ejecutable**.
Cada interprete necesita su ruta exacta y nada mas.