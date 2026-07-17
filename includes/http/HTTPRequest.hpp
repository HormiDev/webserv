#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>

class HTTPRequest
{
public:
	HTTPRequest();
	HTTPRequest(const HTTPRequest &other);
	HTTPRequest &operator=(const HTTPRequest &other);
	~HTTPRequest();

	const std::string &getMethod() const;
	const std::string &getPath() const;
	const std::string &getVersion() const;
	const std::map<std::string, std::string> &getHeaders() const;
	const std::string &getBody() const;
	void setMethod(const std::string &method);
	void setPath(const std::string &path);
	void setVersion(const std::string &version);
	void setHeader(const std::string &key, const std::string &value);
	void setBody(const std::string &newBody);

private:
	std::string _method;
	std::string _path;
	std::string _version;
	std::map<std::string, std::string> _headers;
	std::string _body;
};

#endif