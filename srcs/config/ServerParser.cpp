/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:49 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/08/03 02:39:54 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerParser.hpp"
#include <iterator>
#include <stdexcept>

/**
 * Construye un servidor sin directivas.
 */
ServerParser::ServerParser() : _directives()
{
}

/**
 * Construye una copia de otro servidor.
 */
ServerParser::ServerParser(const ServerParser &other) : _directives(other._directives)
{
}

/**
 * Sustituye las directivas actuales por las de otro servidor.
 */
ServerParser &ServerParser::operator=(const ServerParser &other)
{
	if (this != &other)
		_directives = other._directives;
	return *this;
}

/**
 * Destruye el servidor y sus directivas.
 */
ServerParser::~ServerParser()
{
}

/**
 * Recorre un bloque de servidor y recoge cada una de sus directivas.
 */
ServerParser::ServerParser(const std::vector<Token> &tokens,
	std::vector<Token>::const_iterator iterator) : _directives()
{
	while (iterator != tokens.end())
	{
		if (iterator->getType() == 1 && iterator->getValue() == "}")
			return;
		if (iterator->getType() == 1)
		{
			++iterator;
			if (iterator == tokens.end())
				break;
			if (iterator->getType() == 1 &&
				iterator->getValue() == "}")
				return;
			size_t index = static_cast<size_t>(
				std::distance(tokens.begin(), iterator));
			_directives.push_back(Directives(tokens, index));
		}
		++iterator;
	}
	throw std::runtime_error("Error: server block without closing brace");
}

/**
 * Devuelve la lista de directivas del servidor.
 */
const std::vector<Directives> &ServerParser::getDirectives() const
{
	return _directives;
}

/**
 * Sustituye la lista completa de directivas del servidor.
 */
void ServerParser::setDirectives(const std::vector<Directives> &directives)
{
	_directives = directives;
}
