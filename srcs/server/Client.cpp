/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 12:42:34 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/15 13:02:41 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "server/Client.hpp"
#include "colors.hpp"

/**
 * Constructor for the Client class.
 * 
 * @param clientFd The file descriptor for the client socket.
 */
Client::Client(int clientFd) : _fd(clientFd), _bytesSent(0), _keepAlive(false)
{
	std::cout << BOLD_GREEN << "Client constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the Client class.
 * 
 * @param other The Client object to copy from.
 */
Client::Client(const Client &other)
	: _fd(other._fd), _recvBuffer(other._recvBuffer), _sendBuffer(other._sendBuffer),
	  _bytesSent(other._bytesSent), _request(other._request), _response(other._response),
	  _keepAlive(other._keepAlive)
{
	std::cout << BOLD_GREEN << "Client copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the Client class.
 * 
 * @param other The Client object to assign from.
 * @return A reference to the assigned Client object.
 */
Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_recvBuffer = other._recvBuffer;
		_sendBuffer = other._sendBuffer;
		_bytesSent = other._bytesSent;
		_request = other._request;
		_response = other._response;
		_keepAlive = other._keepAlive;
	}
	std::cout << BOLD_GREEN << "Client assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the Client class.
 */
Client::~Client()
{
	std::cout << BOLD_RED << "Client destructor called" << RESET << std::endl;
}

/**
 * Get the file descriptor for the client socket.
 * 
 * @return The file descriptor for the client socket.
 */
int Client::getFd() const
{
	return _fd;
}

/**
 * Get the receive buffer for the client.
 * 
 * @return A reference to the receive buffer string.
 */
std::string &Client::getRecvBuffer()
{
	return _recvBuffer;
}

/**
 * Get the receive buffer for the client (const version).
 * 
 * @return A const reference to the receive buffer string.
 */
const std::string &Client::getRecvBuffer() const
{
	return _recvBuffer;
}

/**
 * Get the send buffer for the client.
 * 
 * @return A reference to the send buffer string.
 */
std::string &Client::getSendBuffer()
{
	return _sendBuffer;
}

/**
 * Get the send buffer for the client (const version).
 * 
 * @return A const reference to the send buffer string.
 */
const std::string &Client::getSendBuffer() const
{
	return _sendBuffer;
}

/**
 * Get the number of bytes sent to the client.
 * 
 * @return The number of bytes sent to the client.
 */
size_t Client::getBytesSent() const
{
	return _bytesSent;
}

/**
 * Get the HTTP request for the client.
 * 
 * @return A reference to the HTTP request object.
 */
HTTPRequest &Client::getRequest()
{
	return _request;
}

/**
 * Get the HTTP request for the client (const version).
 * 
 * @return A const reference to the HTTP request object.
 */
const HTTPRequest &Client::getRequest() const
{
	return _request;
}

/**
 * Get the HTTP response for the client.
 * 
 * @return A reference to the HTTP response object.
 */
HTTPResponse &Client::getResponse()
{
	return _response;
}

/**
 * Get the HTTP response for the client (const version).
 * 
 * @return A const reference to the HTTP response object.
 */
const HTTPResponse &Client::getResponse() const
{
	return _response;
}

/**
 * Check if the client connection should be kept alive.
 * 
 * @return True if the connection should be kept alive, false otherwise.
 */
bool Client::isKeepAlive() const
{
	return _keepAlive;
}

/**
 * Set whether the client connection should be kept alive.
 * 
 * @param keepAlive True to keep the connection alive, false otherwise.
 */
void Client::setKeepAlive(bool keepAlive)
{
	_keepAlive = keepAlive;
}

/**
 * Add bytes to the count of bytes sent to the client.
 * 
 * @param bytes The number of bytes to add.
 */
void Client::addBytesSent(size_t bytes)
{
	_bytesSent += bytes;
}

/**
 * Reset the count of bytes sent to the client.
 */
void Client::resetBytesSent()
{
	_bytesSent = 0;
}

/**
 * Clear the receive buffer for the client.
 */
void Client::clearRecvBuffer()
{
	_recvBuffer.clear();
}

/**
 * Clear the send buffer for the client.
 */
void Client::clearSendBuffer()
{
	_sendBuffer.clear();
}

/**
 * Reset the client state, including buffers and request/response objects.
 */
void Client::reset()
{
	_recvBuffer.clear();
	_sendBuffer.clear();
	_bytesSent = 0;
	_request = HTTPRequest();
	_response = HTTPResponse();
	_keepAlive = false;
}