/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 15:28:35 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/14 14:57:07 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Config.hpp"
#include <fstream>
#include <sstream>
#include <vector>

/**
 * Default constructor for the Config class. Initializes default values for
 * port, root, and index.
 * 
 * port: Default port number (8080).
 * root: Default root directory ("./").
 * index: Default index file ("index.html").
 */
Config::Config() : _root("./"), _index("index.html"), _port(8080)
{
	std::cout << BOLD_GREEN << "Config default constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the Config class. Creates a new Config object as a copy
 * of another.
 */
Config::Config(const Config& other) : _root(other._root), _index(other._index), _port(other._port)
{
	std::cout << BOLD_GREEN << "Config copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the Config class. Assigns values from another Config
 * object to this one.
 */
Config& Config::operator=(const Config& other)
{
	if (this != &other)
	{
		_root = other._root;
		_index = other._index;
		_port = other._port;
	}
	std::cout << BOLD_GREEN << "Config assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the Config class. Cleans up resources if necessary.
 */
Config::~Config()
{
	std::cout << BOLD_RED << "Config destructor called" << RESET << std::endl;
}

/**
 * Gets the root directory.
 * @return The root directory.
 */
std::string Config::getRoot() const
{
	return _root;
}

/**
 * Gets the index file.
 * @return The index file.
 */
std::string Config::getIndex() const
{
	return _index;
}

/**
 * Gets the port number.
 * @return The port number.
 */
int Config::getPort() const
{
	return _port;
}

/**
 * Sets the root directory.
 * @param root The root directory.
 */
void Config::setRoot(const std::string& root)
{
	_root = root;
}

/**
 * Sets the index file.
 * @param index The index file.
 */
void Config::setIndex(const std::string& index)
{
	_index = index;
}

/**
 * Sets the port number.
 * @param port The port number.
 */
void Config::setPort(int port)
{
	_port = port;
}