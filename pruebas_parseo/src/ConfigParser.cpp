/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:01:12 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/08/01 12:10:34 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.hpp"
#include "src/ConfigParser.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>

/**
 * Construye un parser de configuración vacío.
 */
ConfigParser::ConfigParser()
{
	std::cout << BOLD_GREEN << "ConfigParser default constructor called"
			  << RESET << std::endl;
}

/**
 * Construye una copia de otro parser de configuración.
 */
ConfigParser::ConfigParser(const ConfigParser &other)
	: _fileContent(other._fileContent), _tokenizer(other._tokenizer),
	  _tokens(other._tokens), _servers(other._servers)
{
	std::cout << BOLD_GREEN << "ConfigParser copy constructor called"
			  << RESET << std::endl;
}

/**
 * Sustituye el estado actual por el de otro parser de configuración.
 */
ConfigParser &ConfigParser::operator=(const ConfigParser &other)
{
	if (this != &other)
	{
		_fileContent = other._fileContent;
		_tokenizer = other._tokenizer;
		_tokens = other._tokens;
		_servers = other._servers;
	}
	std::cout << BOLD_GREEN << "ConfigParser assignment operator called"
			  << RESET << std::endl;
	return *this;
}

/**
 * Destruye el parser y sus estructuras internas.
 */
ConfigParser::~ConfigParser()
{
	std::cout << BOLD_RED << "ConfigParser destructor called"
			  << RESET << std::endl;
}

/**
 * Lee el contenido completo de un fichero y lo devuelve como una cadena.
 */
std::string ConfigParser::readFile(const std::string &filename) const
{
	std::ifstream configFile(filename.c_str());

	if (!configFile.is_open())
		throw std::runtime_error("Error opening config file: " + filename);
	std::stringstream buffer;
	buffer << configFile.rdbuf();
	return buffer.str();
}

/**
 * Construye y devuelve una configuración por cada bloque server.
 */
std::vector<Config> ConfigParser::parse(const std::string &filename)
{
	_fileContent = readFile(filename);
	_tokens.clear();
	_tokenizer.tokenize(_fileContent);
	_tokens = _tokenizer.getTokens();
	_servers.clear();
	buildServers();
	return buildConfigs();
}

/**
 * Devuelve la lista de tokens generada durante el parseo.
 * debug
 */
const std::vector<Token> &ConfigParser::getTokens() const
{
	return _tokens;
}

/**
 * Devuelve la lista de servidores construida durante el parseo.
 * debug
 */
const std::vector<Server> &ConfigParser::getServers() const
{
	return _servers;
}

/**
 * Recorre los tokens y construye un servidor por cada token server.
 */
void ConfigParser::buildServers()
{
	std::vector<Token>::const_iterator iterator = _tokens.begin();

	while (iterator != _tokens.end())
	{
		if (iterator->getType() == 2 &&
			iterator->getValue() == "server")
		{
			_servers.push_back(Server(_tokens, iterator));
			while (iterator != _tokens.end() &&
				!(iterator->getType() == 1 &&
				  iterator->getValue() == "}"))
				++iterator;
		}
		if (iterator != _tokens.end())
			++iterator;
	}
}

/**
 * Construye un Config por cada servidor y aplica todas sus directivas.
 */
std::vector<Config> ConfigParser::buildConfigs() const
{
	std::vector<Config> configs;

	for (size_t i = 0; i < _servers.size(); ++i)
	{
		Config config;
		const std::vector<Directives> &directives =
			_servers[i].getDirectives();

		for (size_t j = 0; j < directives.size(); ++j)
		{
			const std::string &name = directives[j].getDirective();
			const std::vector<std::string> &arguments =
				directives[j].getArguments();

			if (name == "listen")
			{
				if (arguments.size() != 1)
					throw std::runtime_error("Error: listen requires one argument");
				std::string listen = arguments[0];
				size_t colon = listen.find(":");
				if (colon != std::string::npos)
				{
					if (colon == 0 || colon == listen.size() - 1)
						throw std::runtime_error("Error: invalid listen argument: " + listen);
					config.setHost(listen.substr(0, colon));
					listen = listen.substr(colon + 1);
				}
				long port = strToLong(listen);
				if (port < 1 || port > 65535)
					throw std::runtime_error("Error: invalid port number: " + listen);
				config.setPort(static_cast<int>(port));
			}
			else if (name == "root")
			{
				if (arguments.size() != 1)
					throw std::runtime_error("Error: root requires one argument");
				config.setRoot(arguments[0]);
			}
			else if (name == "index")
			{
				if (arguments.size() != 1)
					throw std::runtime_error("Error: index requires one argument");
				config.setIndex(arguments[0]);
			}
			else if (name == "error_page")
			{
				if (arguments.size() != 2)
					throw std::runtime_error("Error: error_page requires two arguments");
				long errorCode = strToLong(arguments[0]);
				if (errorCode < 100 || errorCode > 599)
					throw std::runtime_error("Error: invalid error code: " + arguments[0]);
				config.setErrorPage(static_cast<int>(errorCode),
					arguments[1]);
			}
			else if (name == "client_max_body_size")
			{
				if (arguments.size() != 1)
					throw std::runtime_error("Error: client_max_body_size requires one argument");
				std::string value = arguments[0];
				char unit = value[value.size() - 1];
				if (unit == 'K' || unit == 'k' ||
					unit == 'M' || unit == 'm')
					value.erase(value.size() - 1);
				long maxBodySize = strToLong(value);
				if (unit == 'K' || unit == 'k')
					maxBodySize *= 1024L;
				else if (unit == 'M' || unit == 'm')
					maxBodySize *= 1024L * 1024L;
				config.setClientMaxBodySize(maxBodySize);
			}
			else
				throw std::runtime_error("Error: unsupported directive: " + name);
		}
		configs.push_back(config);
	}
	return configs;
}

/**
 * Convierte una cadena numérica en un valor long.
 */
long ConfigParser::strToLong(const std::string &str) const
{
	char *end;
	long value = std::strtol(str.c_str(), &end, 10);

	if (*end != '\0')
		throw std::runtime_error("Invalid number: " + str);
	return value;
}
