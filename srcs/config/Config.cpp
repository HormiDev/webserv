/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 15:28:35 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/21 15:20:55 by mvidal-h         ###   ########.fr       */
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
 * host: Default host ("0.0.0.0").
 */
Config::Config() : _root("./"), _index("index.html"), _host("0.0.0.0"), _port(8080)
{
	std::cout << BOLD_GREEN << "Config default constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the Config class. Creates a new Config object as a copy
 * of another.
 */
Config::Config(const Config& other) : _root(other._root), _index(other._index), _host(other._host), _port(other._port), _errorPages(other._errorPages)
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
		_host = other._host;
		_errorPages = other._errorPages;
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

/*
 * Gets the error page path for a specific error code.
 * @param errorCode The HTTP error code.
 * @return The path to the corresponding error page, or an empty string if not set.
 */
std::string Config::getErrorPage(int errorCode) const
{
	std::map<int, std::string>::const_iterator it = _errorPages.find(errorCode);
	if (it != _errorPages.end())
		return it->second;
	return "";
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

/**
 * Gets the host.
 * @return The host.
 */
std::string Config::getHost() const
{
	return _host;
}

/**
 * Sets the host.
 * @param host The host.
 */
void Config::setHost(const std::string& host)
{
	_host = host;
}

/**
 * Sets the error page path for a specific error code.
 * @param errorCode The HTTP error code.
 * @param errorPagePath The path to the corresponding error page.
 */
void Config::setErrorPage(int errorCode, const std::string& errorPagePath)
{
	_errorPages.insert(std::make_pair(errorCode, errorPagePath));
}

/**
 * Prints the configuration values.
 */
void Config::print() const
{
	std::cout << BOLD_GREEN << "Config values:" << RESET << std::endl;
	std::cout << "  Root: " << _root << std::endl;
	std::cout << "  Index: " << _index << std::endl;
	std::cout << "  Port: " << _port << std::endl;
	std::cout << "  Host: " << _host << std::endl;
	std::cout << "  Error Pages:" << std::endl;
	for (std::map<int, std::string>::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it)
		std::cout << "    " << it->first << ": " << it->second << std::endl;
}