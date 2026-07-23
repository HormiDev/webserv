# CGI

Modulo CGI autocontenido. No depende de `Config`, `HTTPRequest` ni `Server` asique sin asustarse
Se integra ya cuando toque.

## Que hay aqui

- `CgiHandler.hpp` / `CgiHandler.cpp`: fork + pipes + execve, no bloqueante,
  pensado para vivir dentro del `poll()` unico del servidor.

## Como se usa (visto desde fuera)

```cpp
CgiHandler::Request req;
req.method = "GET";                        // viene de HTTPRequest::getMethod()
req.scriptPath = "./www/cgi-bin/hello.py";  // root + path, resuelto por Config
req.queryString = "name=Ismael";            // parte de la URL tras el '?'
req.body = request.getBody();               // ya des-chunkeado si aplica

CgiHandler cgi;
cgi.start(req, "/usr/bin/python3");         // interprete, o "" si el script tiene shebang
```

El servidor (Networking) tiene que:

1. Meter `cgi.getStdinFd()` y `cgi.getStdoutFd()` en su `vector<pollfd>`.
2. En cada vuelta de `poll()`:
   - Si `getStdinFd()` marca `POLLOUT` -> llamar a `cgi.writeToStdin()`.
   - Si `getStdoutFd()` marca `POLLIN` -> llamar a `cgi.readFromStdout()`.
   - Llamar a `cgi.checkTimeout()` periodicamente para no colgarse nunca.
3. Cuando `cgi.isComplete()` sea `true`:
   - Sacar `cgi.getRawOutput()`.
   - Llamar a `CgiHandler::splitOutput(raw, headers, body)`.
   - Construir el `HTTPResponse` real con eso.
   - Quitar los dos fds del `vector<pollfd>` (ya estan cerrados internamente).

## Puntos de integracion pendientes (cuando Config/Server esten cerrados)

- `LocationConfig` necesita: extension CGI (`.py`, `.php`...) y ruta del
  interprete. `Handler` decide si la request es CGI comparando la
  extension del path contra eso.
- `Client` necesita un estado extra (`WAITING_CGI`) y guardar un
  `CgiHandler` mientras dura la ejecucion, para no bloquear al resto de
  clientes mientras se espera al proceso.
- El body que le pasa `Client`/`Handler` a `CgiHandler::Request::body`
  debe llegar ya sin chunked-encoding (el subject lo exige explicitamente:
  el CGI espera EOF plano, no chunks).

## Verificado

Compila limpio con `c++ -Wall -Wextra -Werror -std=c++98`. Probado end to
end con un script Python real (fork, pipes, execve, env vars, lectura no
bloqueante y parseo de la salida).
