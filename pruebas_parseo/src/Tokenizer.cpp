/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:30 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/07/30 06:09:35 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/Tokenizer.hpp"
#include <iostream>
#include <stdexcept>

/**
 * Construye un tokenizador con una lista de tokens vacía.
 */
Tokenizer::Tokenizer()
{
}

/**
 * Construye un tokenizador copiando los tokens de otro.
 */
Tokenizer::Tokenizer(const Tokenizer &other) : _tokens(other._tokens)
{
}

/**
 * Sustituye los tokens actuales por los de otro tokenizador.
 */
Tokenizer &Tokenizer::operator=(const Tokenizer &other)
{
	if (this != &other)
		_tokens = other._tokens;
	return *this;
}

/**
 * Destruye el tokenizador y sus tokens asociados.
 */
Tokenizer::~Tokenizer()
{
}

/**
 * Ejecuta todas las comprobaciones sobre la lista final de tokens.
 */
void Tokenizer::check_tokens() const
{
	check_braces();
	check_tokens_outside_braces();
	check_server_opening_brace();
	check_semicolon_has_string();
}

/**
 * Comprueba el cierre, anidamiento y contenido de los bloques entre llaves.
 */
void Tokenizer::check_braces() const
{
	bool	openBrace = false;
	int		stringCount;

	for (size_t i = 0; i < _tokens.size(); ++i)
	{
		if (_tokens[i].getType() == 1)
		{
			if (_tokens[i].getValue() == "{")
			{
				if (openBrace)
					throw std::runtime_error("Error: nested opening brace in configuration file");
				openBrace = true;
				stringCount = 0;
			}
			else if (_tokens[i].getValue() == "}")
			{
				if (!openBrace)
					throw std::runtime_error("Error: closing brace without opening brace");
				openBrace = false;
				if (stringCount == 0)
					throw std::runtime_error("Error: empty block in configuration file");
			}
		}
		else if (_tokens[i].getType() == 2)
			++stringCount;
	}
	if (openBrace)
		throw std::runtime_error("Error: opening brace without closing brace");
}

/**
 * Verifica que fuera de los bloques solo aparezca el token server.
 */
void Tokenizer::check_tokens_outside_braces() const
{
	bool insideBraces = false;

	for (size_t i = 0; i < _tokens.size(); ++i)
	{
		if (_tokens[i].getType() == 1 &&
			_tokens[i].getValue() == "{")
		{
			insideBraces = true;
			continue;
		}
		if (_tokens[i].getType() == 1 &&
			_tokens[i].getValue() == "}")
		{
			insideBraces = false;
			continue;
		}
		if (!insideBraces && !(_tokens[i].getType() == 2 && _tokens[i].getValue() == "server"))
			throw std::runtime_error("Error: only the server token is allowed outside braces");
	}
}

/**
 * Comprueba que cada token server esté seguido de una llave de apertura.
 */
void Tokenizer::check_server_opening_brace() const
{
	for (size_t i = 0; i < _tokens.size(); ++i)
	{
		if (_tokens[i].getType() == 2 &&
			_tokens[i].getValue() == "server")
		{
			if (i == _tokens.size() - 1 ||
				_tokens[i + 1].getType() != 1 ||
				_tokens[i + 1].getValue() != "{")
				throw std::runtime_error("Error: server token must be followed by an opening brace");
		}
	}
}

/**
 * Comprueba que cada punto y coma tenga antes al menos dos tokens de texto.
 */
void Tokenizer::check_semicolon_has_string() const
{
	size_t stringCount = 0;

	for (size_t i = 0; i < _tokens.size(); ++i)
	{
		if (_tokens[i].getType() == 2)
			++stringCount;
		else if (_tokens[i].getType() == 1 &&
			_tokens[i].getValue() == ";")
		{
			if (stringCount < 2)
				throw std::runtime_error("Error: semicolon must be preceded by at least two string tokens");
			stringCount = 0;
		}
		else if (_tokens[i].getType() == 1 &&
			(_tokens[i].getValue() == "{" ||
			 _tokens[i].getValue() == "}"))
			stringCount = 0;
	}
}

/**
 * Indica si un carácter debe separarse como token especial.
 */
bool Tokenizer::is_special_char(char c) const
{
	return (c == '{' || c == '}' || c == ';' || c == '\n' ||
			c == '\t' || c == ' ' || c == '\r' || c == '#' ||
			c == '"' || c == '\'' || c == '\\');
}

/**
 * Reinicia y ejecuta las fases de creación y compresión de tokens.
 */
void Tokenizer::tokenize(const std::string &fileContent)
{
	_tokens.clear();
	splitTokens(fileContent);
	compressTokens();
	check_tokens();
}

/**
 * Divide el contenido del fichero en tokens de texto y tokens especiales.
 */
void Tokenizer::splitTokens(const std::string &fileContent)
{
	std::string tokenContent;

	for (size_t i = 0; i < fileContent.size(); ++i)
	{
		if (is_special_char(fileContent[i]))
		{
			if (!tokenContent.empty())
			{
				_tokens.push_back(Token(tokenContent, 2));
				tokenContent.clear();
			}
			_tokens.push_back(Token(std::string(1, fileContent[i]), 1));
		}
		else
			tokenContent += fileContent[i];
	}
	if (!tokenContent.empty())
		_tokens.push_back(Token(tokenContent, 2));
}

/**
 * Elimina comentarios y espacios, y agrupa las cadenas entrecomilladas.
 */
void Tokenizer::compressTokens()
{
	std::vector<Token> compressedTokens;

	for (size_t i = 0; i < _tokens.size(); ++i)
	{
		if (_tokens[i].getType() != 1)
		{
			compressedTokens.push_back(_tokens[i]);
			continue;
		}
		if (_tokens[i].getValue() == "#")
		{
			skipComment(i);
			continue;
		}
		if (isWhitespace(_tokens[i]))
			continue;
		if (_tokens[i].getValue() == "\"")
		{
			compressDoubleQuotedString(i, compressedTokens);
			continue;
		}
		if (_tokens[i].getValue() == "'")
		{
			compressSingleQuotedString(i, compressedTokens);
			continue;
		}
		compressedTokens.push_back(_tokens[i]);
	}
	_tokens = compressedTokens;
}

/**
 * Avanza la posición hasta el final de la línea comentada.
 */
void Tokenizer::skipComment(size_t &position) const
{
	while (position < _tokens.size() &&
		_tokens[position].getValue() != "\n")
		++position;
}

/**
 * Indica si el token representa un carácter de espacio en blanco.
 */
bool Tokenizer::isWhitespace(const Token &token) const
{
	return (token.getValue() == "\n" || token.getValue() == "\t" ||
		token.getValue() == " " || token.getValue() == "\r");
}

/**
 * Agrupa una cadena entre comillas dobles y procesa sus escapes.
 */
void Tokenizer::compressDoubleQuotedString(size_t &position,
	std::vector<Token> &compressedTokens) const
{
	++position;
	std::string quotedString;

	while (position < _tokens.size() &&
		!(_tokens[position].getType() == 1 &&
			_tokens[position].getValue() == "\""))
	{
		if (_tokens[position].getType() == 1 &&
			_tokens[position].getValue() == "\\")
		{
			++position;
			if (position >= _tokens.size())
				throw std::runtime_error("Error: unclosed quote in configuration file");
		}
		quotedString += _tokens[position].getValue();
		++position;
	}
	if (position >= _tokens.size())
		throw std::runtime_error("Error: unclosed quote in configuration file");
	compressedTokens.push_back(Token(quotedString, 2));
}

/**
 * Agrupa una cadena entre comillas simples y procesa sus escapes.
 */
void Tokenizer::compressSingleQuotedString(size_t &position,
	std::vector<Token> &compressedTokens) const
{
	++position;
	std::string quotedString;

	while (position < _tokens.size() &&
		!(_tokens[position].getType() == 1 &&
			_tokens[position].getValue() == "'"))
	{
		if (_tokens[position].getType() == 1 &&
			_tokens[position].getValue() == "\\")
		{
			++position;
			if (position >= _tokens.size())
				throw std::runtime_error("Error: unclosed quote in configuration file");
		}
		quotedString += _tokens[position].getValue();
		++position;
	}
	if (position >= _tokens.size())
		throw std::runtime_error("Error: unclosed quote in configuration file");
	compressedTokens.push_back(Token(quotedString, 2));
}

/**
 * Imprime el índice, tipo y contenido de todos los tokens.
 */
void Tokenizer::printTokens() const
{
	for (size_t i = 0; i < _tokens.size(); ++i)
		std::cout	<< "Token[" << i << "]: \""
					<< "\" (type " << _tokens[i].getType() << ") "
					<< _tokens[i].getValue() << std::endl;
}

/**
 * Valida y devuelve una referencia constante a la lista de tokens.
 */
const std::vector<Token> &Tokenizer::getTokens() const
{
	return _tokens;
}
