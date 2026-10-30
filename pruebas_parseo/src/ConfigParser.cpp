/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:01:12 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/06 14:31:05 by ide-dieg         ###   ########.fr       */
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
const std::vector<ServerParser> &ConfigParser::getServers() const
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
			std::vector<Token>::const_iterator serverBegin = iterator;
			int braceCount = 1;
			++iterator;
			// Salta la llave de apertura del server, ya incluida en el contador.
			if (iterator != _tokens.end())
				++iterator;
			while (iterator != _tokens.end() && braceCount > 0)
			{
				if (iterator->getType() == 1)
				{
					if (iterator->getValue() == "{")
						++braceCount;
					else if (iterator->getValue() == "}")
						--braceCount;
				}
				if (braceCount > 0)
					++iterator;
			}
			if (braceCount != 0)
				throw std::runtime_error("Error: server block without closing brace");
			_servers.push_back(ServerParser(_tokens, serverBegin, iterator));
		}
		if (iterator != _tokens.end())
			++iterator;
	}
	check_listen_and_servername();
}

/**
 * Rechaza escuchas solapadas entre servidores con el mismo server_name.
 * Los servidores sin server_name comparten el nombre vacío por defecto.
 */
void ConfigParser::check_listen_and_servername() const
{
	// Compara cada pareja una sola vez: j empieza después de i.
	// Este chequeo compara servidores distintos, no escuchas del mismo bloque.
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		const std::string serverName = _servers[i].getServerName();
		const std::vector<Directives> &firstDirectives = _servers[i].getDirectives();
		for (size_t j = i + 1; j < _servers.size(); ++j)
		{
			// Nombres distintos pueden compartir una escucha.
			if (serverName != _servers[j].getServerName())
				continue;
			const std::vector<Directives> &secondDirectives = _servers[j].getDirectives();
			// Consulta las directivas originales por referencia, sin copiar vectores.
			for (size_t a = 0; a < firstDirectives.size(); ++a)
			{
				if (firstDirectives[a].getDirective() != "listen")
					continue;
				// Usa los mismos valores por defecto que buildConfigs.
				const Listen first = parseListen(firstDirectives[a].getArguments()[0]);
				for (size_t b = 0; b < secondDirectives.size(); ++b)
				{
					if (secondDirectives[b].getDirective() != "listen")
						continue;
					const Listen second = parseListen(secondDirectives[b].getArguments()[0]);
					// Solo hay solapamiento si el puerto coincide y las IP son
					// iguales, o alguna es 0.0.0.0 (abarca cualquier IP).
					if (first.getPort() == second.getPort() &&
						(first.getInterface() == second.getInterface() ||
						 first.getInterface() == "0.0.0.0" ||
						 second.getInterface() == "0.0.0.0"))
					{
						// Detiene el parseo e identifica los servidores (desde 1),
						// el nombre compartido y las escuchas que causan el conflicto.
						std::ostringstream message;
						message << "Error: servers " << i + 1 << " and " << j + 1
							<< " share server_name \"" << serverName
							<< "\" and overlapping listen endpoints: "
							<< first.getInterface() << ":" << first.getPort()
							<< " and " << second.getInterface() << ":"
							<< second.getPort();
						throw std::runtime_error(message.str());
					}
				}
			}
		}
	}
}

/**
 * Convierte un argumento listen validado en una IP y un puerto.
 * Comparte los valores por defecto entre el chequeo y buildConfigs.
 */
Listen ConfigParser::parseListen(const std::string &argument) const
{
	std::string interface = "0.0.0.0";
	long port = 80;
	size_t colon = argument.find(":");

	if (colon != std::string::npos)
	{
		if (colon == 0 || colon == argument.size() - 1)
			throw std::runtime_error("Error: invalid listen argument: " + argument);
		interface = argument.substr(0, colon);
		port = strToLong(argument.substr(colon + 1));
	}
	else if (argument.find('.') != std::string::npos)
		interface = argument;
	else
		port = strToLong(argument);
	if (port < 1 || port > 65535)
		throw std::runtime_error("Error: invalid port number: " + argument);
	return Listen(static_cast<int>(port), interface);
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
				config.addListen(parseListen(arguments[0]));
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
			else if (name == "server_name")
			{
				if (arguments.size() != 1)
					throw std::runtime_error("Error: server_name requires one argument");
				config.setServerName(arguments[0]);
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
