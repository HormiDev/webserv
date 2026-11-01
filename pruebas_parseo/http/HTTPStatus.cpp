/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 12:51:31 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/30 15:51:53 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPStatus.hpp"

/**
 * Returns the status message for a given HTTP status code.
 * 
 * status: The HTTP status code.
 * @return The corresponding status message.
 */
std::string getStatusMessage(HTTPStatus status)
{
	switch (status)
	{
	case OK:
		return "OK";
	case CREATED:
		return "Created";
	case NO_CONTENT:
		return "No Content";
	case MOVED_PERMANENTLY:
		return "Moved Permanently";
	case BAD_REQUEST:
		return "Bad Request";
	case FORBIDDEN:
		return "Forbidden";
	case NOT_FOUND:
		return "Not Found";
	case METHOD_NOT_ALLOWED:
		return "Method Not Allowed";
	case REQUEST_TIMEOUT:
		return "Request Timeout";
	case LENGTH_REQUIRED:
		return "Length Required";
	case CONTENT_TOO_LARGE:
		return "Content Too Large";
	case URI_TOO_LONG:
		return "URI Too Long";
	case INTERNAL_SERVER_ERROR:
		return "Internal Server Error";
	case NOT_IMPLEMENTED:
		return "Not Implemented";
	case SERVICE_UNAVAILABLE:
		return "Service Unavailable";
	case HTTP_VERSION_NOT_SUPPORTED:
		return "HTTP Version Not Supported";
	default:
		return "Unknown Status Code";
	}
}

HTTPStatus stringToHTTPStatus(const std::string &statusString)
{
	if (statusString == "200")
		return OK;
	else if (statusString == "201")
		return CREATED;
	else if (statusString == "204")
		return NO_CONTENT;
	else if (statusString == "301")
		return MOVED_PERMANENTLY;
	else if (statusString == "400")
		return BAD_REQUEST;
	else if (statusString == "403")
		return FORBIDDEN;
	else if (statusString == "404")
		return NOT_FOUND;
	else if (statusString == "405")
		return METHOD_NOT_ALLOWED;
	else if (statusString == "408")
		return REQUEST_TIMEOUT;
	else if (statusString == "411")
		return LENGTH_REQUIRED;
	else if (statusString == "413")
		return CONTENT_TOO_LARGE;
	else if (statusString == "414")
		return URI_TOO_LONG;
	else if (statusString == "500")
		return INTERNAL_SERVER_ERROR;
	else if (statusString == "501")
		return NOT_IMPLEMENTED;
	else if (statusString == "503")
		return SERVICE_UNAVAILABLE;
	else if (statusString == "505")
		return HTTP_VERSION_NOT_SUPPORTED;
	throw std::runtime_error("Invalid HTTP status code: " + statusString);
}