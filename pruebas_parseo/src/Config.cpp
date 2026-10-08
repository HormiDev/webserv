/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 15:28:35 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/06 14:39:02 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/Config.hpp"
/**
 * Default constructor for the Config class. Initializes default values for
 * root and index, and starts without listening endpoints.
 *
 * root: Default root directory ("html/").
 * index: Default index file ("index.html").
 * server_name: Default server name ("").
 * _listens: Default listening endpoint ("0.0.0.0:80").
 * _client_max_body_size: Default maximum client body size (1 MB).
 */
Config::Config() : _root("html/"), _index("index.html"), _server_name(""),
	_listens(), _client_max_body_size(1024L * 1024L)
{
	std::cout << BOLD_GREEN << "Config default constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the Config class. Creates a new Config object as a copy
 * of another.
 */
Config::Config(const Config &other)
	: _root(other._root), _index(other._index),
	  _server_name(other._server_name), _listens(other._listens),
	  _errorPages(other._errorPages),
	  _client_max_body_size(other._client_max_body_size)
{
	std::cout << BOLD_GREEN << "Config copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the Config class. Assigns values from another Config
 * object to this one.
 */
Config &Config::operator=(const Config &other)
{
	if (this != &other)
	{
		_root = other._root;
		_index = other._index;
		_server_name = other._server_name;
		_listens = other._listens;
		_errorPages = other._errorPages;
		_client_max_body_size = other._client_max_body_size;
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
 * Gets the maximum accepted request body size in bytes.
 */
long Config::getClientMaxBodySize() const
{
	return _client_max_body_size;
}

/**
 * Sets the root directory.
 * @param root The root directory.
 */
void Config::setRoot(const std::string &root)
{
	_root = root;
}

/**
 * Sets the index file.
 * @param index The index file.
 */
void Config::setIndex(const std::string &index)
{
	_index = index;
}

/**
 * Gets every listening endpoint configured for this server.
 */
const std::vector<Listen> &Config::getListens() const
{
	return _listens;
}

/**
 * Gets the server name.
 * @return The server name, or an empty string if it was not configured.
 */
std::string Config::getServerName() const
{
	return _server_name;
}

/**
 * Adds a listening endpoint to this server.
 */
void Config::addListen(const Listen &listen)
{
	_listens.push_back(listen);
}

/**
 * Sets the server name.
 * @param serverName The server name.
 */
void Config::setServerName(const std::string &serverName)
{
	_server_name = serverName;
}

/**
 * Sets the error page path for a specific error code.
 * @param errorCode The HTTP error code.
 * @param errorPagePath The path to the corresponding error page.
 */
void Config::setErrorPage(int errorCode, const std::string &errorPagePath)
{
	_errorPages.insert(std::make_pair(errorCode, errorPagePath));
}

/**
 * Sets the maximum accepted request body size in bytes.
 */
void Config::setClientMaxBodySize(long clientMaxBodySize)
{
	_client_max_body_size = clientMaxBodySize;
}

/**
 * Prints the configuration values.
 */
void Config::print() const
{
	std::cout << BOLD_GREEN << "Config values:" << RESET << std::endl;
	std::cout << "  Root: " << _root << std::endl;
	std::cout << "  Index: " << _index << std::endl;
	std::cout << "  Listens:" << std::endl;
	for (size_t i = 0; i < _listens.size(); ++i)
		std::cout << "    [" << i << "] "
			<< _listens[i].getInterface() << ":"
			<< _listens[i].getPort() << std::endl;
	std::cout << "  Server name: " << _server_name << std::endl;
	std::cout << "  Client max body size: "
		<< _client_max_body_size << " bytes" << std::endl;
	std::cout << "  Error Pages:" << std::endl;
	for (std::map<int, std::string>::const_iterator it = _errorPages.begin();
		 it != _errorPages.end(); ++it)
		std::cout << "    " << it->first << ": " << it->second << std::endl;
}
