/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 14:54:36 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/14 14:57:34 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HTTPRequest.hpp"
#include <iostream>
#include "colors.hpp"

/**
 * Default constructor for the HTTPRequest class.
 */
HTTPRequest::HTTPRequest() : _method(""), _path(""), _version("")
{
	std::cout << BOLD_GREEN << "HTTPRequest default constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the HTTPRequest class.
 * 
 * other: The HTTPRequest object to copy.
 */
HTTPRequest::HTTPRequest(const HTTPRequest& other)
	: _method(other._method),
	  _path(other._path),
	  _version(other._version),
	  _headers(other._headers),
	  _body(other._body)
{
	std::cout << BOLD_GREEN << "HTTPRequest copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the HTTPRequest class.
 * 
 * other: The HTTPRequest object to assign.
 * @return A reference to the assigned object.
 */
HTTPRequest& HTTPRequest::operator=(const HTTPRequest& other) {
	if (this != &other) {
		_method = other._method;
		_path = other._path;
		_version = other._version;
		_headers = other._headers;
		_body = other._body;
	}
	std::cout << BOLD_GREEN << "HTTPRequest assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the HTTPRequest class.
 */
HTTPRequest::~HTTPRequest()
{
	std::cout << BOLD_RED << "HTTPRequest destructor called" << RESET << std::endl;
}

/**
 * Returns the HTTP method of the request.
 * 
 * @return A reference to the HTTP method string.
 */
const std::string& HTTPRequest::getMethod() const {
	return _method;
}

/**
 * Returns the path of the request.
 * 
 * @return A reference to the path string.
 */
const std::string& HTTPRequest::getPath() const {
	return _path;
}

/**
 * Returns the HTTP version of the request.
 * 
 * @return A reference to the HTTP version string.
 */
const std::string& HTTPRequest::getVersion() const {
	return _version;
}

/**
 * Returns the headers of the request.
 * 
 * @return A reference to the headers map.
 */
const std::map<std::string, std::string>& HTTPRequest::getHeaders() const {
	return _headers;
}

/**
 * Returns the body of the request.
 * 
 * @return A reference to the body string.
 */
const std::string& HTTPRequest::getBody() const {
	return _body;
}

/**
 * Sets the HTTP method of the request.
 * 
 * method: The HTTP method string to set.
 */
void HTTPRequest::setMethod(const std::string& method) {
	_method = method;
}

/**
 * Sets the path of the request.
 *
 * path: The path string to set.
 */
void HTTPRequest::setPath(const std::string& path) {
	_path = path;
}

/**
 * Sets the HTTP version of the request.
 *
 * version: The HTTP version string to set.
 */
void HTTPRequest::setVersion(const std::string& version) {
	_version = version;
}

/**
 * Sets a header for the request.
 *
 * key: The header key.
 * value: The header value.
 */
void HTTPRequest::setHeader(const std::string& key, const std::string& value) {
	_headers[key] = value;
}

/**
 * Sets the body of the request.
 *
 * newBody: The new body string.
 */
void HTTPRequest::setBody(const std::string& newBody) {
	_body = newBody;
}