/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseSerializer.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 15:51:51 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/10 15:53:49 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSESERIALIZER_HPP
#define HTTPRESPONSESERIALIZER_HPP

#include "HTTPResponse.hpp"

class HTTPResponseSerializer
{
	public:
		HTTPResponseSerializer();
		HTTPResponseSerializer(const HTTPResponseSerializer& other);
		HTTPResponseSerializer& operator=(const HTTPResponseSerializer& other);
		~HTTPResponseSerializer();

		std::string serialize(const HTTPResponse& response) const;

	private:
		std::string serializeStatusLine(const HTTPResponse& response) const;
		std::string serializeHeaders(const HTTPResponse& response) const;
};

#endif