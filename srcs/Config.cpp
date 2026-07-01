/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:18:50 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/07/01 13:44:14 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

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
Config::Config(const Config& other) : _filename(other._filename), _root(other._root), _index(other._index), _port(other._port)
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
		_filename = other._filename;
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
 * Loads configuration from a file.
 * @param filename The name of the file to load configuration from.
 */
void Config::load(const std::string& filename)
{
	_filename = filename;
	std::cout << BOLD_YELLOW << "Loading configuration from: " << filename << RESET << std::endl;

	//code to load configuration from file (not implemented)
}

/**
 * Constructor for the Config class. Initializes the filename and calls load.
 * @param filename The name of the file to load configuration from.
 */
Config::Config(const std::string& filename) : _filename(filename), _root("./"), _index("index.html"), _port(8080)
{
	std::cout << BOLD_GREEN << "Config constructor with filename called" << RESET << std::endl;
	load(filename);
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
