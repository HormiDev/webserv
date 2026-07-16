/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 12:14:40 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/16 11:43:36 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"

class Client
{
	private:
		int _fd;
		std::string _recvBuffer;
		std::string _sendBuffer;
		size_t _bytesSent;
		HTTPRequest _request;
		HTTPResponse _response;
		bool _keepAlive;

	public:
		Client(int clientFd);
		Client(const Client& other);
		Client& operator=(const Client& other);
		~Client();

		// Getters
		int getFd() const;

		std::string& getRecvBuffer();
		const std::string& getRecvBuffer() const;

		std::string& getSendBuffer();
		const std::string& getSendBuffer() const;

		size_t getBytesSent() const;

		HTTPRequest& getRequest();
		const HTTPRequest& getRequest() const;

		HTTPResponse& getResponse();
		const HTTPResponse& getResponse() const;

		bool isKeepAlive() const;

		// Setters / acciones
		void setKeepAlive(bool keepAlive);

		void addBytesSent(size_t bytes);
		void resetBytesSent();

		void clearRecvBuffer();
		void clearSendBuffer();

		void reset();
};

#endif