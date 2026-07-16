#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include "http/HTTPRequest.hpp"
#include <vector>

class HTTPRequestParser
{
public:
	HTTPRequestParser();
	HTTPRequestParser(const HTTPRequestParser &other);
	HTTPRequestParser &operator=(const HTTPRequestParser &other);
	~HTTPRequestParser();

	HTTPRequest parse(const std::string &rawRequest);
	bool isRequestComplete(const std::string &buffer);

private:
	void parseRequestLine(HTTPRequest &request, const std::string &rawRequest);
	void parseMethod(HTTPRequest &request, const std::string &method);
	void parsePath(HTTPRequest &request, const std::string &path);
	void parseVersion(HTTPRequest &request, const std::string &version);
	// void				parseHeaders(HTTPRequest& request, const std::string& rawRequest);
	// void				parseBody(HTTPRequest& request, const std::string& rawRequest);
};

#endif