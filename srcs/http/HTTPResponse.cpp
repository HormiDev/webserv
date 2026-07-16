/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 12:50:27 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/15 17:43:36 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HTTPResponse.hpp"
#include <iostream>
#include <sstream>
#include "colors.hpp"

/**
 * Default constructor for the HTTPResponse class.
 * 
 * Initializes the status code to 200 (OK) and the status message to "OK".
 */
HTTPResponse::HTTPResponse() : _statusCode(200), _statusMessage("OK")
{
	std::cout << BOLD_GREEN << "HTTPResponse default constructor called" << std::endl;
}

/**
 * Copy constructor for the HTTPResponse class.
 * 
 * @param other The HTTPResponse object to copy from.
 */
HTTPResponse::HTTPResponse(const HTTPResponse &other)
	: _statusCode(other._statusCode), _statusMessage(other._statusMessage),
	  _headers(other._headers), _body(other._body)
{
	std::cout << BOLD_GREEN << "HTTPResponse copy constructor called" << std::endl;
}

/**
 * Assignment operator for the HTTPResponse class.
 * 
 * @param other The HTTPResponse object to assign from.
 * @return A reference to the assigned HTTPResponse object.
 */
HTTPResponse &HTTPResponse::operator=(const HTTPResponse &other)
{
	if (this != &other)
	{
		_statusCode = other._statusCode;
		_statusMessage = other._statusMessage;
		_headers = other._headers;
		_body = other._body;
	}
	std::cout << BOLD_GREEN << "HTTPResponse assignment operator called" << std::endl;
	return *this;
}

/**
 * Destructor for the HTTPResponse class.
 */
HTTPResponse::~HTTPResponse()
{
	std::cout << BOLD_RED << "HTTPResponse destructor called" << std::endl;
}

/**
 * Get the status code of the HTTP response.
 * 
 * @return The status code.
 */
int HTTPResponse::getStatusCode() const
{
	return _statusCode;
}

/**
 * Get the status message of the HTTP response.
 * 
 * @return The status message.
 */
const std::string &HTTPResponse::getStatusMessage() const
{
	return _statusMessage;
}

/**
 * Get the headers of the HTTP response.
 * 
 * @return A constant reference to the headers map.
 */
const std::map<std::string, std::string> &HTTPResponse::getHeaders() const
{
	return _headers;
}

/**
 * Get the body of the HTTP response.
 * 
 * @return A constant reference to the body string.
 */
const std::string &HTTPResponse::getBody() const
{
	return _body;
}

/**
 * Set the status code of the HTTP response.
 * 
 * @param code The new status code.
 */
void HTTPResponse::setStatusCode(int code)
{
	_statusCode = code;
}

/**
 * Set the status message of the HTTP response.
 * 
 * @param message The new status message.
 */
void HTTPResponse::setStatusMessage(const std::string &message)
{
	_statusMessage = message;
}

/**
 * Set a header in the HTTP response.
 * 
 * @param key The header key.
 * @param value The header value.
 */
void HTTPResponse::setHeader(const std::string &key, const std::string &value)
{
	_headers[key] = value;
}

/**
 * Set the body of the HTTP response.
 * 
 * @param newBody The new body content.
 */
void HTTPResponse::setBody(const std::string &newBody)
{
	_body = newBody;
}

/**
 * Serializes an HTTP response into a string.
 * 
 * @return The serialized string representing the HTTP response.
 */
std::string HTTPResponse::serialize() const
{
	std::string serializedResponse;
	serializedResponse += serializeStatusLine();
	serializedResponse += serializeHeaders();
	serializedResponse += "\r\n"; // Blank line to separate headers from body
	serializedResponse += _body;
	return serializedResponse;
}

/**
 * Serializes the status line of an HTTP response.
 * 
 * @return The serialized status line string.
 */
std::string HTTPResponse::serializeStatusLine() const
{
	std::ostringstream serializedStatusStream;
	serializedStatusStream << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";
	return serializedStatusStream.str();
}

/**
 * Serializes the headers of an HTTP response transforming them into a string.
 * 
 * @return The serialized headers string.
 */
std::string HTTPResponse::serializeHeaders() const
{
	std::ostringstream serializedHeadersStream;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
		 it != _headers.end(); ++it)
	{
		serializedHeadersStream << it->first << ": " << it->second << "\r\n";
	}
	return serializedHeadersStream.str();
}