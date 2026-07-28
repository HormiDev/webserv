/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:15:20 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/24 11:48:40 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "config/Config.hpp"

class ServerSocket
{
private:
	Config _config;
	struct addrinfo *_addrInfo;
	int _serverSocketFd;

	//Configuration and socket setup
	void setupAddressInfo();
	void freeAddressInfo();
	void createSocket();
	void bindSocket();
	void listenSocket();

public:
	ServerSocket(const Config &config);
	ServerSocket(const ServerSocket &other);
	ServerSocket &operator=(const ServerSocket &other);
	~ServerSocket();

	//getters
	int getserverSocketFd() const;
	const Config &getConfig() const;

	void startServerSocket();
};

#endif // SERVER_SOCKET_HPP