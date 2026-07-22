/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:34:35 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/22 16:57:29 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // close()
#include <sstream>	// std::ostringstream
#include <cstdlib>	// exit()
#include <netdb.h>	// getaddrinfo(), freeaddrinfo()

#include "server/ServerSocket.hpp"
#include <cstring>

/**
 * Constructor for the ServerSocket class.
 *
 * @param config The configuration object containing server settings.
 */
ServerSocket::ServerSocket(const Config &config)
	: _config(config), _addrInfo(NULL), _serverSocketFd(-1)
{
	std::cout << BOLD_GREEN << "ServerSocket constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the ServerSocket class.
 *
 * @param other The ServerSocket object to copy from.
 */
ServerSocket::ServerSocket(const ServerSocket &other)
	: _config(other._config), _addrInfo(other._addrInfo), _serverSocketFd(other._serverSocketFd)
{
	std::cout << BOLD_GREEN << "ServerSocket copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the ServerSocket class.
 *
 * @param other The ServerSocket object to assign from.
 * @return A reference to the assigned ServerSocket object.
 */
ServerSocket &ServerSocket::operator=(const ServerSocket &other)
{
	if (this != &other)
	{
		_config = other._config;
		_addrInfo = other._addrInfo;
		_serverSocketFd = other._serverSocketFd;
	}
	std::cout << BOLD_GREEN << "ServerSocket assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the ServerSocket class. It closes the server socket to free up resources.
 */
ServerSocket::~ServerSocket()
{
	if (_serverSocketFd != -1)
		close(_serverSocketFd);
	std::cout << BOLD_RED << "ServerSocket destructor called" << RESET << std::endl;
}

/**
 * Sets up the address information for the server socket.
 */
void ServerSocket::setupAddressInfo()
{
	addrinfo
		hints; // Estructura que contiene información sobre el tipo de socket que queremos crear. La usamos para indicarle al sistema operativo qué tipo de socket queremos crear y cómo queremos que se comporte.
	std::ostringstream portStream;

	portStream << _config.getPort();
	std::string port = portStream.str();

	memset(&hints, 0, sizeof(hints));

	hints.ai_family =
		AF_INET; // AF_INET for IPv4, AF_INET6 for IPv6, AF_UNSPEC for any address family
	hints.ai_socktype = SOCK_STREAM; // TCP socket
	hints.ai_flags = AI_PASSIVE;	 // Socket will be used for binding

	if (getaddrinfo(_config.getHost().c_str(), port.c_str(), &hints, &_addrInfo) !=
		0) // (Parametros: host, port, hints(La receta de cómo queremos crear el socket), result (la estructura donde se guardará la info de la dirección. Mirar en http.md para entender la estructura addrinfo))
	{
		std::cerr << "Error getting address info" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * Frees the address information for the server socket.
 */
void ServerSocket::freeAddressInfo()
{
	if (_addrInfo)
	{
		freeaddrinfo(_addrInfo);
		_addrInfo = NULL;
	}
}

/**
 * Creates the server socket.
 */
void ServerSocket::createSocket()
{
	_serverSocketFd = socket(_addrInfo->ai_family, _addrInfo->ai_socktype, _addrInfo->ai_protocol);
	std::cout
		<< "Socket descriptor: " << _serverSocketFd
		<< std::
			   endl; //IMP: Se podria borrar. solo era para debugueary entender bien el flujo de la creacion del socket.
	if (_serverSocketFd == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * Binds the server socket to the specified port and address.
 */
void ServerSocket::bindSocket()
{
	if (bind(_serverSocketFd, _addrInfo->ai_addr, _addrInfo->ai_addrlen) == -1)
	{
		std::cerr << "Error binding socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Socket bound to port " << _config.getPort() << std::endl;
}

/**
 * Puts the server socket into listening mode to accept incoming connections.
 */
void ServerSocket::listenSocket()
{
	if (listen(_serverSocketFd, SOMAXCONN) ==
		-1) //IMP: no se si aqui ira SOMAXCONN o un numero fijo de conexiones pendientes. SOMAXCONN es una constante que representa el número máximo de conexiones pendientes que el sistema operativo permite para un socket. Usar SOMAXCONN permite que el sistema operativo determine automáticamente un valor adecuado basado en sus propias limitaciones y configuraciones internas. Esto es útil porque garantiza que el servidor pueda manejar tantas conexiones como sea posible sin exceder los límites del sistema.
	{
		std::cerr << "Error listening on socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Listening for incoming connections..." << std::endl;
}

/**
 * Gets the server socket descriptor.
 *
 * @return The server socket descriptor.
 */
int ServerSocket::getFd() const
{
	return _serverSocketFd;
}

/**
 * Gets the configuration object associated with the server socket.
 *
 * @return A reference to the configuration object.
 */
const Config &ServerSocket::getConfig() const
{
	return _config;
}

/**
 * Initializes the server socket by setting up address information, creating the socket, binding it, and putting it into listening mode.
 */
void ServerSocket::startServerSocket()
{
	setupAddressInfo();
	createSocket();
	bindSocket();
	freeAddressInfo(); // Free the address info after binding the socket to avoid memory leaks
	listenSocket();
}