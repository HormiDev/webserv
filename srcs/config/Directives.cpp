/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:10:01 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/08/03 02:35:04 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Directives.hpp"
#include <stdexcept>

/**
 * Construye una directiva vacía y sin argumentos.
 */
Directives::Directives() : _directive(""), _arguments()
{
}

/**
 * Construye una copia de otra directiva.
 */
Directives::Directives(const Directives &other)
	: _directive(other._directive), _arguments(other._arguments)
{
}

/**
 * Sustituye la directiva y los argumentos por los de otra instancia.
 */
Directives &Directives::operator=(const Directives &other)
{
	if (this != &other)
	{
		_directive = other._directive;
		_arguments = other._arguments;
	}
	return *this;
}

/**
 * Destruye la directiva y sus argumentos.
 */
Directives::~Directives()
{
}

/**
 * Recoge una directiva y sus argumentos desde una lista de tokens.
 */
Directives::Directives(const std::vector<Token> &tokens, size_t index)
	: _directive(""), _arguments()
{
	if (index >= tokens.size())
		throw std::out_of_range("Directive index is out of range");
	if (tokens[index].getType() != 2)
		throw std::runtime_error("Directive must start with a type 2 token");

	_directive = tokens[index].getValue();
	++index;
	while (index < tokens.size() && tokens[index].getType() != 1)
	{
		_arguments.push_back(tokens[index].getValue());
		++index;
	}
	check_directive();
}

/**
 * Devuelve el nombre de la directiva.
 */
const std::string &Directives::getDirective() const
{
	return _directive;
}

/**
 * Devuelve la lista de argumentos de la directiva.
 */
const std::vector<std::string> &Directives::getArguments() const
{
	return _arguments;
}

/**
 * Establece el nombre de la directiva.
 */
void Directives::setDirective(const std::string &directive)
{
	_directive = directive;
}

/**
 * Sustituye la lista completa de argumentos de la directiva.
 */
void Directives::setArguments(const std::vector<std::string> &arguments)
{
	_arguments = arguments;
}

/**
 * Comprueba que la directiva exista y delega su validación específica.
 */
void Directives::check_directive() const
{
	if (_directive.empty())
		throw std::runtime_error("Directive cannot be empty");
	if (_directive != "listen" && _directive != "root" &&
		_directive != "index" && _directive != "error_page" &&
		_directive != "client_max_body_size")
		throw std::runtime_error("Error: invalid directive: " + _directive);
	if (_directive == "index")
		check_index();
	else if (_directive == "listen")
		check_listen();
	else if (_directive == "root")
		check_root();
	else if (_directive == "error_page")
		check_error_page();
	else if (_directive == "client_max_body_size")
		check_client_max_body_size();
}

/**
 * Comprueba que la directiva index tenga al menos un argumento, que
 * ninguno esté vacío y que no exceda la longitud máxima de ruta.
 */
void Directives::check_index() const
{
	if (_arguments.empty() || _arguments.size() < 1)
		throw std::runtime_error("Directive index requires at least two arguments");
	for (size_t i = 0; i < _arguments.size(); ++i)
	{
		if (_arguments[i].empty())
			throw std::runtime_error("Directive index cannot have empty arguments");
		if (_arguments[i].length() > 4096) // longitud maxima de ruta por defecto en linux
			throw std::runtime_error("Directive index argument exceeds maximum path length");
	}
}

/**
 * Comprueba que la directiva listen tenga exactamente un argumento y que sea válido.
 * de momento solo se permite un argumento, pero se puede ampliar para permitir varios.
 * pero lo he dejado asi para que se adapte a la estructura de datos de Config.
 */
void Directives::check_listen() const
{
	if (_arguments.empty() || _arguments.size() != 1)
		throw std::runtime_error("Directive listen requires exactly one argument");
	for (size_t i = 0; i < _arguments.size(); ++i)
	{
		if (_arguments[i].empty())
			throw std::runtime_error("Directive listen cannot have empty arguments");
		check_listen_argument(_arguments[i]);
	}
}

/**
 * Comprueba que un argumento de listen sea IPv4, puerto o IPv4:puerto.
 */
void Directives::check_listen_argument(const std::string &argument) const
{
	if (!is_ipv4(argument) && !is_port(argument) &&
		!is_ipv4_port(argument))
		throw std::runtime_error("Invalid listen argument: " + argument);
}

/**
 * Comprueba el formato de una dirección IPv4 y el rango de sus octetos.
 */
bool Directives::is_ipv4(const std::string &argument) const
{
	if (argument.empty() || argument[0] == '.' ||
		argument[argument.size() - 1] == '.')
		return false;
	size_t start = 0;
	size_t octetCount = 0;

	while (start < argument.size())
	{
		size_t end = argument.find('.', start);
		if (end == std::string::npos)
			end = argument.size();
		if (end == start || end - start > 3)
			return false;
		int value = 0;
		for (size_t i = start; i < end; ++i)
		{
			if (argument[i] < '0' || argument[i] > '9')
				return false;
			value = value * 10 + (argument[i] - '0');
		}
		if (value > 255)
			return false;
		++octetCount;
		if (end == argument.size())
			break;
		start = end + 1;
	}
	return octetCount == 4;
}

/**
 * Comprueba que un puerto sea numérico y esté entre 1 y 65535.
 */
bool Directives::is_port(const std::string &argument) const
{
	if (argument.empty() || argument.size() > 5)
		return false;
	long port = 0;
	for (size_t i = 0; i < argument.size(); ++i)
	{
		if (argument[i] < '0' || argument[i] > '9')
			return false;
		port = port * 10 + (argument[i] - '0');
		if (port > 65535)
			return false;
	}
	return port >= 1;
}

/**
 * Comprueba el formato combinado IPv4:puerto.
 */
bool Directives::is_ipv4_port(const std::string &argument) const
{
	size_t colon = argument.find(':');

	if (colon == std::string::npos || argument.find(':', colon + 1) != std::string::npos)
		return false;
	return is_ipv4(argument.substr(0, colon)) && is_port(argument.substr(colon + 1));
}

/**
 * Comprueba que la directiva root tenga exactamente una ruta.
 * Y que la ruta no esté vacía ni exceda la longitud máxima de ruta.
 * Revisar posible fallo de caracter nulo /0
 */
void Directives::check_root() const
{
	if (_arguments.size() != 1)
		throw std::runtime_error("Directive root requires exactly one argument");
	if (_arguments[0].empty())
		throw std::runtime_error("Directive root path cannot be empty");
	if (_arguments[0].size() > 4096)
		throw std::runtime_error("Directive root path exceeds maximum path length");
}

/**
 * Comprueba los argumentos de la directiva error_page.
 * De momento solo puede recivir un código de error y una ruta,
 * pero se puede ampliar para permitir varios códigos de error y una ruta.
 * Se comprueba que el código de error sea numérico, de tres dígitos entre 300 y 599,
 * y que la ruta no esté vacía ni exceda la longitud máxima de ruta.
 */
void Directives::check_error_page() const
{
	if (_arguments.size() != 2)
		throw std::runtime_error("Directive error_page requires exactly two arguments");
	if (_arguments[0].size() != 3)
		throw std::runtime_error("Directive error_page requires a three-digit status code");
	int errorCode = 0;
	for (size_t i = 0; i < _arguments[0].size(); ++i)
	{
		if (_arguments[0][i] < '0' || _arguments[0][i] > '9')
			throw std::runtime_error("Directive error_page status code must be numeric");
		errorCode = errorCode * 10 + (_arguments[0][i] - '0');
	}
	if (errorCode < 300 || errorCode > 599)
		throw std::runtime_error("Directive error_page status code must be between 300 and 599");
	if (_arguments[1].empty())
		throw std::runtime_error("Directive error_page path cannot be empty");
	if (_arguments[1].size() > 4096)
		throw std::runtime_error("Directive error_page path exceeds maximum path length");
}

/**
 * Comprueba el formato del argumento de client_max_body_size.
 * Acepta un número entero positivo seguido opcionalmente de K, M.
 * Se comprueba que el valor no exceda 1 GiB.
 */
void Directives::check_client_max_body_size() const
{
	const unsigned long long limit = 1024ULL * 1024ULL * 1024ULL;
	unsigned long long maxSize = 0;

	if (_arguments.size() != 1)
		throw std::runtime_error("Directive client_max_body_size requires exactly one argument");
	const std::string &argument = _arguments[0];
	if (argument.empty())
		throw std::runtime_error("Directive client_max_body_size cannot be empty");
	size_t numberLength = argument.size();
	char unit = argument[argument.size() - 1];
	if (unit == 'K' || unit == 'k')
		--numberLength;
	else if (unit == 'M' || unit == 'm')
		--numberLength;
	if (numberLength == 0)
		throw std::runtime_error("Directive client_max_body_size requires a numeric value");
	for (size_t i = 0; i < numberLength; ++i)
	{
		if (argument[i] < '0' || argument[i] > '9')
			throw std::runtime_error("Invalid client_max_body_size value: " + argument);
		maxSize = maxSize * 10 + (argument[i] - '0');
		if (maxSize > limit)
			throw std::runtime_error("Directive client_max_body_size cannot exceed 1 GiB");
	}
	if (unit == 'K' || unit == 'k')
		maxSize *= 1024ULL;
	else if (unit == 'M' || unit == 'm')
		maxSize *= 1024ULL * 1024ULL;
	if (maxSize > limit)
		throw std::runtime_error("Directive client_max_body_size cannot exceed 1 GiB");
	if (maxSize == 0)
		throw std::runtime_error("Directive client_max_body_size must be greater than zero");
}
