/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 12:51:31 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/14 14:57:59 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpStatus.hpp"

/**
 * Returns the status message for a given HTTP status code.
 * 
 * status: The HTTP status code.
 * @return The corresponding status message.
 */
std::string getStatusMessage(HttpStatus status)
{
	switch (status)
	{
	case OK:
		return "OK";
	case CREATED:
		return "Created";
	case NO_CONTENT:
		return "No Content";
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
