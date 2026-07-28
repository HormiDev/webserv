#!/usr/bin/env python3
# Script CGI minimo de prueba para el prototipo de CgiHandler.
# Lo uso para comprobar: variables de entorno CGI/1.1, lectura del body
# por stdin (POST) y que las cabeceras + body de salida se separan bien.

import os
import sys

method = os.environ.get("REQUEST_METHOD", "GET")
query = os.environ.get("QUERY_STRING", "")

body = ""
content_length = os.environ.get("CONTENT_LENGTH", "")
if content_length:
    body = sys.stdin.read(int(content_length))

# Cabeceras CGI (el servidor las separa del body buscando la linea en blanco).
print("Content-Type: text/plain")
print()
print("Hola desde hello.py")
print("METHOD=%s" % method)
print("QUERY_STRING=%s" % query)
print("BODY=%s" % body)
