# http format

## peticion vasica del usuario
```url
http://server_ip:puerto/hola_mundo.html
```

## http recivido por el servidor
```http
Request: GET /hola_mundo.html HTTP/1.1
Host: localhost:8080
Connection: keep-alive
sec-ch-ua: "Not;A=Brand";v="8", "Chromium";v="150", "Google Chrome";v="150"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Linux"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/150.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: es-ES,es;q=0.9
```

## hola_mundo.html
```html
<!DOCTYPE html>
<html lang="en">
<head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Webserv</title>
</head>
<body>
        <h1>¡HolaMundo!</h1>
        <p>Hola estás en la página <strong>helloWorld.html</strong>.</p>

        <hr>

        <p><strong>Proyecto:</strong> webserv (42)</p>
        <p><strong>Servidor:</strong> Tu implementación en C++98</p>
</body>
</html>
```

## respuesta del servidor
```http
HTTP/1.1 200 OK
Content-Length: 400
Content-Type: text/html

<!DOCTYPE html>
<html lang="en">
<head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Webserv</title>
</head>
<body>
        <h1>¡HolaMundo!</h1>
        <p>Hola estás en la página <strong>helloWorld.html</strong>.</p>

        <hr>

        <p><strong>Proyecto:</strong> webserv (42)</p>
        <p><strong>Servidor:</strong> Tu implementación en C++98</p>
</body>
</html>
```

## addrinfo: Para configurar el serversocket. La funcion getaddrinfo() devuelve una lista de estructuras addrinfo que contienen información sobre las direcciones que se pueden usar para crear un socket. La estructura addrinfo contiene la siguiente información:
```
struct addrinfo
{
    int ai_family; // la familia de direcciones que se va a usar. AF_INET para IPv4, AF_INET6 para IPv6 y AF_UNSPEC para cualquier familia de direcciones.
    int ai_socktype; // el tipo de socket que se va a usar. SOCK_STREAM para TCP, SOCK_DGRAM para UDP y SOCK_RAW para raw sockets.
    int ai_protocol; // el protocolo que se va a usar. IPPROTO_TCP para TCP, IPPROTO_UDP para UDP y IPPROTO_IP para cualquier protocolo. Lo genera getaddrinfo() en función de la familia de direcciones y el tipo de socket que se le pase. 

    socklen_t ai_addrlen; // la longitud de la dirección que se va a usar. Se usa para indicar al sistema  cuánto espacio se necesita para almacenar la dirección.

    sockaddr *ai_addr; // un puntero a la estructura sockaddr que contiene la dirección que se va a usar. La estructura sockaddr es una estructura genérica que puede contener cualquier tipo de dirección. Se usa para almacenar la dirección del socket. Yo la pienso como una address creada a partir de la configuracion que le hemos pasado a la funcion getaddrinfo() y que es entendida por el sistema operativo para bindear el socket. Es la misma que generabamos antes de hacer el cambio a getaddrinfo() con la estructura sockaddr_in manualmente. La diferencia es que ahora getaddrinfo() nos la genera automáticamente en función de la familia de direcciones y el tipo de socket que le pasemos.

    addrinfo *ai_next; // un puntero a la siguiente estructura addrinfo en la lista. Si cuando llamamos a getaddrinfo en hints le pasamos el flag para todas las interfaces IPv4 y IPv6, nos devolverá una lista de estructuras addrinfo, una por cada interfaz. Este puntero nos permite recorrer la lista de estructuras addrinfo.
};
```
