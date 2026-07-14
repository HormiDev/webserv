/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseSerializer.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 15:53:53 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/14 14:57:54 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HTTPResponseSerializer.hpp"
#include <sstream>

/**
 * Default constructor for the HTTPResponseSerializer class.
 */
HTTPResponseSerializer::HTTPResponseSerializer() {}

/**
 * Copy constructor for the HTTPResponseSerializer class.
 * 
 * @param other The HTTPResponseSerializer object to copy from.
 */
HTTPResponseSerializer::HTTPResponseSerializer(const HTTPResponseSerializer& other) { (void)other; }

/**
 * Assignment operator for the HTTPResponseSerializer class.
 * 
 * @param other The HTTPResponseSerializer object to assign from.
 * @return A reference to the assigned HTTPResponseSerializer object.
 */
HTTPResponseSerializer& HTTPResponseSerializer::operator=(const HTTPResponseSerializer& other) {
	(void)other;
	return *this;
}

/**
 * Destructor for the HTTPResponseSerializer class.
 */
HTTPResponseSerializer::~HTTPResponseSerializer() {}

/**
 * Serializes an HTTP response into a string.
 * 
 * @param response The HTTPResponse object to serialize.
 * @return The serialized string representing the HTTP response.
 */
std::string HTTPResponseSerializer::serialize(const HTTPResponse& response) const
{
	std::string serializedResponse;
	serializedResponse += serializeStatusLine(response);
	serializedResponse += serializeHeaders(response);
	serializedResponse += "\r\n"; // Blank line to separate headers from body
	serializedResponse += response.getBody();
	return serializedResponse;
}

/**
 * Serializes the status line of an HTTP response.
 * 
 * @param response The HTTPResponse object containing the status information.
 * @return The serialized status line string.
 */
std::string HTTPResponseSerializer::serializeStatusLine(const HTTPResponse& response) const
{
	std::ostringstream serializedStatusStream;
	serializedStatusStream	<< "HTTP/1.1 " 
							<< response.getStatusCode() 
							<< " " << response.getStatusMessage() 
							<< "\r\n";
	return serializedStatusStream.str();
}

/**
 * Serializes the headers of an HTTP response transforming them into a string.
 * 
 * @param response The HTTPResponse object containing the headers.
 * @return The serialized headers string.
 */
std::string HTTPResponseSerializer::serializeHeaders(const HTTPResponse& response) const
{
	std::ostringstream serializedHeadersStream;
	const std::map<std::string, std::string>& headers = response.getHeaders();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		serializedHeadersStream << it->first << ": " << it->second << "\r\n";
	}
	return serializedHeadersStream.str();
}