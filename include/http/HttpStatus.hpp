/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 11:17:31 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/14 12:52:59 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_STATUS_HPP
#define HTTP_STATUS_HPP

#include <string>

enum HttpStatus
{
	// 2xx everything is OK
	OK = 200,
	CREATED = 201,
	NO_CONTENT = 204,

	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	REQUEST_TIMEOUT = 408,
	LENGTH_REQUIRED = 411,
	CONTENT_TOO_LARGE = 413,
	URI_TOO_LONG = 414,

	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	SERVICE_UNAVAILABLE = 503,
	HTTP_VERSION_NOT_SUPPORTED = 505
};

std::string getStatusMessage(HttpStatus status);

#endif