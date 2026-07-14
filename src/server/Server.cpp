#include <unistd.h> // close()
#include <cstdlib>
#include <iostream>
#include <fstream> // std::ifstream
#include <sstream> // std::ostringstream
#include <sys/socket.h> // socket(), bind(), listen(), accept(), send(), recv()
#include <netinet/in.h> // sockaddr_in, htons(), ntohs(), INADDR_ANY

#include "server/Server.hpp"
#include <arpa/inet.h> // inet_ntoa(), inet_ntop(), inet_pton(), inet_addr()

#include "http/HTTPRequestParser.hpp"
#include "http/HTTPRequest.hpp"
#include "http/HTTPException.hpp"
#include "http/HTTPResponseSerializer.hpp"

/**
 * Constructor for the Server class.
 * 
 * @param config The configuration object containing server settings.
 */
Server::Server(const Config& config) : _config(config), _serverSocket(-1){}

/**
 * Copy constructor for the Server class.
 * 
 * @param other The Server object to copy from.
 */
Server::Server(const Server& other) : _config(other._config), _serverSocket(other._serverSocket), _pollFds(other._pollFds) {}

/**
 * Assignment operator for the Server class.
 * 
 * @param other The Server object to assign from.
 * @return A reference to the assigned Server object.
 */
Server& Server::operator=(const Server& other)
{
	if (this != &other)
	{
		_config = other._config;
		_serverSocket = other._serverSocket;
		_pollFds = other._pollFds;
	}
	return *this;
}

/**
 * Destructor for the Server class. It closes the server socket to free up resources.
 */
Server::~Server()
{
	close(_serverSocket);
}

/**
 * Starts the server and enters the main loop to handle incoming connections and requests.
 */
void Server::start()
{
	createSocket();
	bindSocket();
	listenSocket();
	
	initPoll();
	runLoop();
}

/**
 * Creates the server socket.
 */
void Server::createSocket()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET for IPv4, SOCK_STREAM for TCP, AF_INET6 for IPv6, SOCK_DGRAM for UDP
	std::cout << "Socket descriptor: " << _serverSocket << std::endl;
	if (_serverSocket == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * Binds the server socket to the specified port and address.
 */
void Server::bindSocket()
{
	// struct sockaddr_in
	// {
	// 	sa_family_t     sin_family;   // IPv4
	// 	in_port_t       sin_port;     // puerto
	// 	struct in_addr  sin_addr;     // IP
	// };
	struct sockaddr_in addr;

	addr.sin_family = AF_INET; // IPv4
	addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces: localhost(127.0.0.1), wi-fi, ethernet, etc.
	addr.sin_port = htons(_config.getPort()); // Port number (htons = Host TO Network Short -> convert to network byte order)

	if (bind(_serverSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		std::cerr << "Error binding socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Socket bound to port " << _config.getPort() << std::endl;
}

/**
 * Puts the server socket into listening mode to accept incoming connections.
 */
void Server::listenSocket()
{
	if (listen(_serverSocket, SOMAXCONN) == -1)
	{
		std::cerr << "Error listening on socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Listening for incoming connections..." << std::endl;
}

/**
 * Initializes the poll structure to monitor the server socket for incoming connections.
 */
void Server::initPoll()
{
	struct pollfd serverPoll; //Un pollfd es una estructura (ya existente en la libreria poll.h) que contiene un descriptor de archivo (fd), los eventos que queremos escuchar (events) y los eventos que han ocurrido (revents).
	serverPoll.fd = _serverSocket;
	serverPoll.events = POLLIN; // En este caso, vamos a escuchar el socket del servidor para ver si hay nuevas conexiones entrantes (POLLIN). Solo se activara si hay una nueva conexion entrante, no si hay datos para leer de un cliente ya conectado.
	serverPoll.revents = 0; // Revents se inicializa a 0 porque aún no han ocurrido eventos. El kernel marcará el revents con el evento que haya ocurrido (si es que ocurre alguno) cuando llamemos a poll().

	_pollFds.push_back(serverPoll); // Agregamos el pollfd del servidor al vector de pollfd que vamos a pasar a la función poll(). De esta manera, el servidor estará escuchando nuevas conexiones entrantes en su socket.
									// El vector _pollFds contendrá todos los pollfd que queremos escuchar: el del servidor y los de los clientes conectados. Cuando llamemos a poll(), el kernel nos dirá cuáles de estos pollfd tienen eventos pendientes (nuevas conexiones o datos para leer).
									// Es como una lista de elementos a los que el poll estará atento y nos avisará cuando haya algo que atender.
}

/**
 * Main loop of the server that waits for events on the monitored file descriptors and handles them accordingly.
 */
void Server::runLoop()
{
	while (true)
	{
		int pollCount = poll(_pollFds.data(), _pollFds.size(), -1); //Activamos el Poll para que empiece a escuchar los eventos. Parametros: array de pollfd, número de fds, timeout (-1 = infinito)
		if (pollCount == -1)
		{
			std::cerr << "Error in poll()" << std::endl;
			continue;
		}
		for (size_t i = 0; i < _pollFds.size(); ++i) //Cuando ocurre un evento en cualquiera de los elementos en los que esta escuchando el poll, se sale y tenemos que recorrerlos con el bucle pasando por todos (incluso los que no tienen eventos activos en su revent.)
		{
			if (_pollFds[i].revents & POLLIN) // Si el elemento tiene un evento pendiente (revents != 0) y ese evento es POLLIN (hay datos para leer o una nueva conexión entrante), entonces entramos en el if. EL & es un and bit a bit, porque el revent se maneja por bits para cada tipo de evento. Si el revent tiene el bit de POLLIN activado, significa que hay datos para leer o una nueva conexión entrante. Por eso se hace en & con el bit del POLLIN que nos asegura que, al menos el bit del POLLIN esta activo.
			{
				std::cout << "------------Event on fd: " << _pollFds[i].fd << std::endl;
				if (_pollFds[i].fd == _serverSocket) //si el elemento es el propio servidor, significa que hay una nueva conexión entrante y tenemos que aceptarla. Si no, significa que es un cliente ya conectado y tenemos que leer los datos que nos envía.
					acceptClient();
				else								//si no es el servidor, significa que es un cliente ya conectado y tenemos que leer los datos que nos envía.
					handleClient(_pollFds[i].fd);
			}
		}
	}
}

/**
 * Accepts a new client connection and adds it to the poll structure for monitoring.
 */
void Server::acceptClient()
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	int clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientSocket == -1)
	{
		std::cerr << "Error accepting client connection" << std::endl;
		return ;
	}
	std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

	struct pollfd clientPoll; //Creamos un nuevo pollfd para el cliente que acabamos de aceptar. Este pollfd nos permitirá escuchar los eventos que ocurran en el socket del cliente, como datos entrantes (POLLIN) o desconexión (POLLHUP).

	clientPoll.fd = clientSocket;
	clientPoll.events = POLLIN; // Wait for incoming data from the client
	clientPoll.revents = 0;

	_pollFds.push_back(clientPoll); // lo añadimos en el vector de elementos a escuchar.
}

/**
 * Handles a client request by parsing it and generating an appropriate response.
 * 
 * @param clientSocket The socket connected to the client.
 */
void Server::handleClient(int clientSocket)
{
	char buffer[4096];

	ssize_t bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0); //Leemos lo que nos envia el cliente
	// bytes > 0: data received, bytes == 0: client disconnected in a good way, bytes < 0: error
	if (bytes <= 0)
	{
		std::cout << "Client disconnected" << std::endl;
		disconnectClient(clientSocket);
		return;
	}

	buffer[bytes] = '\0';
	std::cout << "Request: " << buffer << std::endl; //Linea para mostrar por pantalla la peticion y poder entenderla. IMP: luego podemos quitarla.
	std::string rawRequest(buffer, bytes);

	HTTPRequestParser parser;
	try
	{
		HTTPRequest request = parser.parse(rawRequest); //Parseamos la peticion que hemos obtenido con el recv y la convertimos en un objeto HTTPRequest que nos permite acceder a los diferentes elementos de la peticion (metodo, path, version, headers, body)
		//IMP: QUITAR ESTO DESPUÉS DE HACER PRUEBAS
		// std::cout << "Parsed request: " << std::endl;
		// std::cout << "Method: " << request.getMethod() << std::endl;
		// std::cout << "Path: " << request.getPath() << std::endl;
		// std::cout << "Version: " << request.getVersion() << std::endl;
		// std::cout << "Headers: " << std::endl;
		// for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin(); it != request.getHeaders().end(); ++it)
		// 	std::cout << it->first << ": " << it->second << std::endl;
		// std::cout << "Body: " << request.getBody() << std::endl;
		//IMP: QUITAR ESTO DESPUÉS DE HACER PRUEBAS

		HTTPResponse response = handleRequest(request); // Con la peticion parseada, generamos la respuesta correspondiente. Esto puede implicar leer un archivo del sistema de archivos, o demoas cosas que nos pida el enunciado del proyecto. En este caso, simplemente leemos un archivo y lo devolvemos como respuesta.
		sendResponse(clientSocket, response); // Enviamos la respuesta al cliente. Esto implica serializar el objeto HTTPResponse en un string y enviarlo por el socket del cliente.
		disconnectClient(clientSocket); // Desconectamos al cliente después de enviar la respuesta. Esto cierra el socket del cliente y lo elimina de la estructura de pollfd. En el servidor real, podríamos querer mantener la conexión abierta para permitir múltiples solicitudes/respuestas en la misma conexión (HTTP keep-alive), pero para esteejemplo, simplemente cerramos la conexión después de una solicitud.
	}
	catch (const HTTPException& e)
	{
		HTTPResponse response = createErrorResponse(static_cast<HttpStatus>(e.getStatusCode())); // Si ocurre una excepción HTTP (por ejemplo, un error de parseo de la solicitud), generamos una respuesta de error correspondiente y la enviamos al cliente. Luego tendremos que mejorarlo con las paginas de error personalizadas que nos indicara en archivo de configuracion.
		sendResponse(clientSocket, response);
		disconnectClient(clientSocket);
		return;
	}
}

/**
 * Disconnects a client from the server, closing the socket and removing it from the poll structure.
 * 
 * @param clientSocket The socket connected to the client.
 */
void Server::disconnectClient(int clientSocket)
{
	close(clientSocket);
	for (size_t i = 0; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].fd == clientSocket)
		{
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}
}

/**
 * Sends an HTTP response to the client.
 * 
 * @param clientSocket The socket connected to the client.
 * @param response The HTTP response to send.
 */
void Server::sendResponse(int clientSocket, const HTTPResponse& response)
{
	HTTPResponseSerializer serializer;
	std::string responseString = serializer.serialize(response);
	send(clientSocket, responseString.c_str(), responseString.size(), 0);
}

/**
 * Handles an HTTP request and generates an appropriate response.
 * 
 * @param request The HTTP request to handle.
 * @return The HTTP response to send.
 */
HTTPResponse Server::handleRequest(const HTTPRequest& request)
{
	std::string srcPath;
	if (request.getPath() == "/") // calcula donde esta la pagina html a decolver segun los parametros parseados del archivo conf.
		srcPath = _config.getRoot() + "/" + _config.getIndex(); // el index por defecto
	else
		srcPath = _config.getRoot() + request.getPath(); // la pagina solicitada

	std::ifstream file_stream(srcPath.c_str());
	if (!file_stream.is_open()) // si no puede abrir el fichero o no existe, devolvemos un error 404
	{
		std::ostringstream bodystream;
		bodystream << NOT_FOUND << " " << getStatusMessage(NOT_FOUND);
		std::string body = bodystream.str();
		return createResponse(NOT_FOUND, "text/plain", body);
	}
	else // si puede abrir el fichero, lo leemos y lo devolvemos como respuesta.
	{
		std::ostringstream bodystream;
		bodystream << file_stream.rdbuf();
		std::string body = bodystream.str();
		return createResponse(OK, "text/html", body);
	}
}

/**
 * Creates an HTTP response with the specified status code, content type, and body.
 * 
 * @param statusCode The HTTP status code.
 * @param contentType The content type of the response.
 * @param body The body content of the response.
 * @return The created HTTP response.
 */
HTTPResponse Server::createResponse(HttpStatus statusCode, const std::string& contentType, const std::string& body)
{
	HTTPResponse response;
	std::string statusMessage = getStatusMessage(statusCode);
	
	std::ostringstream lengthstream;
	lengthstream << body.length();
	
	response.setStatusCode(statusCode);
	response.setStatusMessage(statusMessage);
	response.setHeader("Content-Type", contentType);
	response.setHeader("Content-Length", lengthstream.str());
	response.setBody(body);
	return response;
}

/**
 * Creates an error HTTP response with the specified status code.
 * 
 * @param statusCode The HTTP status code.
 * @return The created HTTP response.
 */
HTTPResponse Server::createErrorResponse(HttpStatus statusCode)
{
	std::ostringstream bodystream;
	bodystream << statusCode << " " << getStatusMessage(statusCode);
	std::string body = bodystream.str();
	return createResponse(statusCode, "text/plain", body);
}