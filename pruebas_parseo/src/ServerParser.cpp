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
	check_listen();
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
