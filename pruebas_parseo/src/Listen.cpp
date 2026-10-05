/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 12:23:53 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/09/18 13:14:21 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listen.hpp"

/**
 * Constructor for the Listen class.
 * @param port The port number to listen on.
 * @param interface The network interface to bind to.
 */
Listen::Listen(int port, const std::string &interface) : _port(port), _interface(interface) {}

/**
 * Constructor for the Listen class with only a port number.
 * @param port The port number to listen on.
 */
Listen::Listen(int port) : _port(port), _interface("0.0.0.0") {}


/**
 * Copy constructor for the Listen class.
 * @param other The Listen object to copy from.
 */
Listen::Listen(const Listen &other) : _port(other._port), _interface(other._interface) {}

/**
 * Assignment operator for the Listen class.
 * @param other The Listen object to assign from.
 * @return A reference to the assigned Listen object.
 */
Listen &Listen::operator=(const Listen &other)
{
	if (this != &other)
	{
		_port = other._port;
		_interface = other._interface;
	}
	return *this;
}

/**
 * Equality operator for the Listen class.
 * @param other The Listen object to compare with.
 * @return True if the two Listen objects are equal, false otherwise.
 */
bool Listen::operator==(const Listen &other) const
{
	return (_port == other._port && _interface == other._interface);
}

/**
 * Destructor for the Listen class.
 */
Listen::~Listen() {}

/**
 * Gets the port number.
 * @return The port number.
 */
int Listen::getPort() const
{
	return _port;
}

/**
 * Gets the network interface.
 * @return A constant reference to the network interface string.
 */
const std::string &Listen::getInterface() const
{
	return _interface;
}