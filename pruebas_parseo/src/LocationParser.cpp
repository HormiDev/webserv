/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParser.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:27:51 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/10/07 16:31:48 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/LocationParser.hpp"
#include <stdexcept>

LocationParser::LocationParser() {}

/**
 * Valida la cabecera y los tipos de directivas de un location ya parseado.
 */
LocationParser::LocationParser(const BlockParser &block)
	: _directives(block.getDirectives())
{
	if (block.getName() != "location")
		throw std::runtime_error("Error: expected a location block");
	const std::vector<std::string> &arguments = block.getArguments();
	if (arguments.size() != 1)
		throw std::runtime_error("Error: location requires exactly one path argument");
	if (arguments[0].empty() || arguments[0][0] != '/')
		throw std::runtime_error("Error: location path must start with /");
	if (!block.getBlocks().empty())
		throw std::runtime_error("Error: nested blocks are not allowed inside location");
	check_directive_types();
}

/**
 * Convierte un bloque location en sus valores declarados, sin aplicar herencia.
 * Los valores únicos se sobrescriben en orden y los métodos se acumulan.
 */
Location LocationParser::parse(const BlockParser &block)
{
	const LocationParser parser(block);
	Location location(block.getArguments()[0]);
	for (size_t i = 0; i < parser._directives.size(); ++i)
	{
		const std::string &name = parser._directives[i].getDirective();
		const std::vector<std::string> &arguments = parser._directives[i].getArguments();
		if (name == "root")
			location.setRoot(arguments[0]);
		else if (name == "index")
		{
			if (arguments.size() != 1)
				throw std::runtime_error("Directive index requires exactly one argument inside location");
			location.setIndex(arguments[0]);
		}
		else if (name == "autoindex")
			location.setAutoindex(arguments[0] == "on");
		else if (name == "allowed_methods")
		{
			for (size_t j = 0; j < arguments.size(); ++j)
				location.addAllowedMethod(stringToHTTPMethod(arguments[j]));
		}
		else if (name == "return")
			location.setRedirect(stringToHTTPStatus(arguments[0]), arguments[1]);
		else if (name == "upload_store")
			location.setUploadStore(arguments[0]);
		else if (name == "cgi")
			location.setCgi(arguments[0], arguments[1]);
	}
	return location;
}

/**
 * Comprueba solo los nombres; Directives ya validó sus argumentos.
 */
void LocationParser::check_directive_types() const
{
	for (size_t i = 0; i < _directives.size(); ++i)
	{
		const std::string &name = _directives[i].getDirective();
		if (name != "root" && name != "index" && name != "autoindex" &&
			name != "allowed_methods" && name != "return" &&
			name != "upload_store" && name != "cgi")
			throw std::runtime_error("Error: directive \"" + name +
				"\" is not allowed inside location");
	}
}

LocationParser::LocationParser(const LocationParser &other)
	: _directives(other._directives) {}

LocationParser &LocationParser::operator=(const LocationParser &other)
{
	if (this != &other)
		_directives = other._directives;
	return *this;
}

LocationParser::~LocationParser() {}

