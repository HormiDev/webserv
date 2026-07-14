#ifndef SERVER_HPP
#define SERVER_HPP

#include <poll.h>
#include <vector>
#include "config/Config.hpp"
#include "http/HTTPResponse.hpp"
#include "http/HTTPRequest.hpp"
#include "http/HttpStatus.hpp"
class Server
{
private:
	Config _config;
	int _serverSocket;
	std::vector<struct pollfd> _pollFds;

	//Configuration and socket setup
	void 			createSocket();
	void 			bindSocket();
	void 			listenSocket();
	void 			initPoll();
	void 			runLoop();
	//Client handling
	void 			acceptClient();
	void 			handleClient(int clientSocket);
	void 			disconnectClient(int clientSocket);
	//Request handling
	HTTPResponse	handleRequest(const HTTPRequest& request);
	//Response handling
	HTTPResponse	createResponse(HttpStatus statusCode, const std::string& contentType, const std::string& body);
	HTTPResponse	createErrorResponse(HttpStatus statusCode);
	void 			sendResponse(int clientSocket, const HTTPResponse& response);
	
public:
			Server(const Config& config);
			Server(const Server& other);
	Server&	operator=(const Server& other);
			~Server();

	void start();
};

#endif