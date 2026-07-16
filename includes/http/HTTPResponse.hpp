/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 12:20:07 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/16 11:38:27 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <map>

class HTTPResponse
{
	public:
		HTTPResponse();
		HTTPResponse(const HTTPResponse& other);
		HTTPResponse& operator=(const HTTPResponse& other);
		~HTTPResponse();

		int											getStatusCode() const;
		const std::string&							getStatusMessage() const;
		const std::map<std::string, std::string>&	getHeaders() const;
		const std::string&							getBody() const;
		void										setStatusCode(int code);
		void										setStatusMessage(const std::string& message);
		void										setHeader(const std::string& key, const std::string& value);
		void										setBody(const std::string& newBody);

		std::string									serialize() const;

	private:
		int _statusCode;
		std::string _statusMessage;
		std::map<std::string, std::string> _headers;
		std::string _body;

		std::string serializeStatusLine() const;
		std::string serializeHeaders() const;
};

#endif