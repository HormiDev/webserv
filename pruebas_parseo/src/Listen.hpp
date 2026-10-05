/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 11:21:28 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/09/18 13:13:37 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_HPP
#define LISTEN_HPP

#include <string>

class Listen
{
public:
	Listen(int port, const std::string &interface);
	Listen(int port);
	Listen(const Listen &other);
	Listen &operator=(const Listen &other);
	bool operator==(const Listen &other) const;
	~Listen();

	int getPort() const;
	const std::string &getInterface() const;

private:
	int _port;
	std::string _interface;
};

#endif
