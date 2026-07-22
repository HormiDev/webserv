#ifndef SERVER_HPP
#define SERVER_HPP

#include <poll.h>
#include <vector>
#include <map>
#include "config/Config.hpp" //IMP: revisar si sobra algun include.
#include "http/HTTPResponse.hpp"
#include "http/HTTPRequest.hpp"
#include "http/HTTPRequestParser.hpp"
#include "http/HttpStatus.hpp"
#include "server/Client.hpp"
#include "server/ServerSocket.hpp"
class Server
{
	private:
		// Config						_config;
		// struct addrinfo				*_addrInfo;
		ServerSocket				_serverSocket;
		std::vector<struct pollfd>	_pollFds;
		std::map<int, Client>		_clients;
		HTTPRequestParser			_parser;

		//Configuration and socket setup
		// void 			setupAddressInfo();
		// void			freeAddressInfo();
		// void 			createSocket();
		// void 			bindSocket();
		// void 			listenSocket();
		void 			initPoll();
		void 			runLoop();
		void			setPollEvent(int clientSocket, short events);
		//Client handling
		void 			acceptClient();
		void 			readFromClient(int clientSocket);
		void 			writeToClient(int clientSocket);
		Client&			getClient(int clientSocket);
		void 			disconnectClient(int clientSocket);
		//Request handling
		HTTPResponse	handleRequest(const HTTPRequest& request);
		//Response handling
		HTTPResponse	createResponse(HttpStatus statusCode, const std::string& contentType, const std::string& body);
		HTTPResponse	createErrorResponse(HttpStatus statusCode);
		
	public:
				Server(const Config& config);
				Server(const Server& other);
		Server&	operator=(const Server& other);
				~Server();

		void start();
};

#endif