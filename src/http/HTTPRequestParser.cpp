#include "http/HTTPRequestParser.hpp"
#include <iostream>
#include <sstream>
#include "colors.hpp"
#include "http/HTTPException.hpp"
#include "http/HttpStatus.hpp"

/**
 * Default constructor for the HTTPRequestParser class.
 */
HTTPRequestParser::HTTPRequestParser()
{
	std::cout << BOLD_GREEN << "HTTPRequestParser default constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the HTTPRequestParser class.
 * 
 * other: The HTTPRequestParser object to copy.
 */
HTTPRequestParser::HTTPRequestParser(const HTTPRequestParser& other)
{
	(void)other;
	std::cout << BOLD_GREEN << "HTTPRequestParser copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the HTTPRequestParser class.
 * 
 * other: The HTTPRequestParser object to assign.
 * @return A reference to the assigned object.
 */
HTTPRequestParser& HTTPRequestParser::operator=(const HTTPRequestParser& other)
{
	(void)other;
	std::cout << BOLD_GREEN << "HTTPRequestParser assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the HTTPRequestParser class.
 */
HTTPRequestParser::~HTTPRequestParser()
{
	std::cout << BOLD_RED << "HTTPRequestParser destructor called" << RESET << std::endl;
}

/**
 * Parses the raw HTTP request string and returns an HTTPRequest object.
 * 
 * rawRequest: The raw HTTP request string to parse.
 * @return An HTTPRequest object initialized with the parsed values.
 */
HTTPRequest HTTPRequestParser::parse(const std::string& rawRequest)
{
	HTTPRequest request;
	parseRequestLine(request, rawRequest);
	//IMP: Implement parseHeaders and parseBody methods to handle headers and body parsing.
	return request;
}

/**
 * Parses the request line of the HTTP request.
 * 
 * request: The HTTPRequest object to populate.
 * rawRequest: The raw HTTP request string.
 */
void HTTPRequestParser::parseRequestLine(HTTPRequest& request, const std::string& rawRequest)
{
	std::istringstream rawStream(rawRequest);
	std::string requestLine;

	if (!std::getline(rawStream, requestLine))
		throw HTTPException(BAD_REQUEST);

	std::istringstream requestLineStream(requestLine);
	std::string method, path, version, extra;

	if (!(requestLineStream >> method >> path >> version)) // Intenta leer exactamente los tres campos de la request line. Si alguna lectura falla, el stream entra en estado de error.
		throw HTTPException(BAD_REQUEST);
	if (requestLineStream >> extra) //si hay más datos en el stream, es un error
		throw HTTPException(BAD_REQUEST);

	parseMethod(request, method);
	parsePath(request, path);
	parseVersion(request, version);

}

/**
 * Parses the method of the HTTP request.
 * 
 * request: The HTTPRequest object to populate.
 * method: The method string to parse.
 */
void	HTTPRequestParser::parseMethod(HTTPRequest& request, const std::string& method)
{
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw HTTPException(METHOD_NOT_ALLOWED);
	request.setMethod(method);
}

/**
 * Parses the path of the HTTP request.
 * 
 * request: The HTTPRequest object to populate.
 * path: The path string to parse.
 */
void	HTTPRequestParser::parsePath(HTTPRequest& request, const std::string& path)
{
	if (path.empty() || path[0] != '/')
		throw HTTPException(BAD_REQUEST);
	request.setPath(path);
}

/**
 * Parses the version of the HTTP request.
 * 
 * request: The HTTPRequest object to populate.
 * version: The version string to parse.
 */
void	HTTPRequestParser::parseVersion(HTTPRequest& request, const std::string& version)
{
	//Pensar si aceptamos también HTTP/1.0, o solo HTTP/1.1. Por ahora solo aceptamos HTTP/1.1
	if (version != "HTTP/1.1")
		throw HTTPException(BAD_REQUEST);
	request.setVersion(version);
}
