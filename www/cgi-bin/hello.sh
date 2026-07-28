#!/bin/sh
# Version en shell del mismo script de prueba, para probar el CGI con un
# interprete distinto (sh en vez de python3) sin cambiar nada del CgiHandler.

BODY=""
if [ -n "$CONTENT_LENGTH" ]; then
	BODY=$(head -c "$CONTENT_LENGTH")
fi

echo "Content-Type: text/plain"
echo ""
echo "Hola desde hello.sh"
echo "METHOD=$REQUEST_METHOD"
echo "QUERY_STRING=$QUERY_STRING"
echo "BODY=$BODY"
