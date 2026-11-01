/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:49 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/07/30 06:09:51 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/ServerParser.hpp"
#include "src/LocationParser.hpp"
#include <set>
#include <stdexcept>

/**
 * Construye un servidor sin directivas ni bloques.
 */
ServerParser::ServerParser() : _directives(), _blocks()
{
}

/**
 * Construye una copia de otro servidor.
 */
ServerParser::ServerParser(const ServerParser &other)
	: _directives(other._directives), _blocks(other._blocks)
{
}

/**
 * Sustituye las directivas y bloques actuales por los de otro servidor.
 */
ServerParser &ServerParser::operator=(const ServerParser &other)
{
	if (this != &other)
	{
		_directives = other._directives;
		_blocks = other._blocks;
	}
	return *this;
}

/**
 * Destruye el servidor, sus directivas y sus bloques.
 */
ServerParser::~ServerParser()
{
}

/**
 * Parsea las directivas y bloques hasta el cierre localizado por buildServers.
 */
ServerParser::ServerParser(const std::vector<Token> &tokens,
	std::vector<Token>::const_iterator iterator,
	std::vector<Token>::const_iterator closingBrace) : _directives(), _blocks()
{
	const BlockParser serverBlock(tokens, iterator, closingBrace);
	_directives = serverBlock.getDirectives();
	_blocks = serverBlock.getBlocks();
	check_block_types();
	check_duplicate_locations();
	check_directive_types();
	check_listen();
	for (size_t i = 0; i < _blocks.size(); ++i)
	{
		const LocationParser location(_blocks[i]);
	}
}

/**
 * Comprueba que los bloques hijos del servidor sean de tipo location.
 */
void ServerParser::check_block_types() const
{
	for (size_t i = 0; i < _blocks.size(); ++i)
	{
		if (_blocks[i].getName() != "location")
			throw std::runtime_error("Error: only location blocks are allowed inside server: " +
				_blocks[i].getName());
	}
}

/**
 * Rechaza rutas location repetidas dentro de este servidor.
 */
void ServerParser::check_duplicate_locations() const
{
	std::set<std::string> paths;
	for (size_t i = 0; i < _blocks.size(); ++i)
	{
		const std::vector<std::string> &arguments = _blocks[i].getArguments();
		// LocationParser comprueba las cabeceras sin ruta o con varios argumentos.
		if (arguments.size() != 1)
			continue;
		if (!paths.insert(arguments[0]).second)
			throw std::runtime_error("Error: duplicate location path \"" + arguments[0] +
				"\" inside server");
	}
}

/**
 * Comprueba solo los nombres de las directivas propias del bloque server.
 * Sus argumentos ya fueron validados al construir cada Directives.
 */
void ServerParser::check_directive_types() const
{
	for (size_t i = 0; i < _directives.size(); ++i)
	{
		const std::string &name = _directives[i].getDirective();
		if (name != "listen" && name != "server_name" && name != "root" &&
			name != "index" && name != "error_page" && name != "client_max_body_size")
			throw std::runtime_error("Error: directive \"" + name +
				"\" is not allowed inside server");
	}
}

/**
 * Comprueba que el bloque server tenga al menos una directiva listen.
 */
void ServerParser::check_listen() const
{
	for (size_t i = 0; i < _directives.size(); ++i)
	{
		if (_directives[i].getDirective() == "listen")
			return;
	}
	throw std::runtime_error("Error: server requires at least one listen directive");
}

/**
 * Devuelve la lista de directivas del servidor.
 */
const std::vector<Directives> &ServerParser::getDirectives() const
{
	return _directives;
}

/**
 * Devuelve los bloques internos del servidor.
 */
const std::vector<BlockParser> &ServerParser::getBlocks() const
{
	return _blocks;
}

/**
 * Devuelve el último server_name declarado, o un nombre vacío si se omite.
 */
std::string ServerParser::getServerName() const
{
	// Busca desde el final para respetar el último server_name declarado.
	for (size_t i = _directives.size(); i > 0; --i)
	{
		if (_directives[i - 1].getDirective() == "server_name")
			return _directives[i - 1].getArguments()[0];
	}
	return "";
}

/**
 * Sustituye la lista completa de directivas del servidor.
 */
void ServerParser::setDirectives(const std::vector<Directives> &directives)
{
	_directives = directives;
}
