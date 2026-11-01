#include "src/BlockParser.hpp"
#include <iterator>
#include <stdexcept>

BlockParser::BlockParser() : _name(), _arguments(), _directives(), _blocks()
{
}

BlockParser::BlockParser(const BlockParser &other)
	: _name(other._name), _arguments(other._arguments),
	  _directives(other._directives), _blocks(other._blocks)
{
}

BlockParser &BlockParser::operator=(const BlockParser &other)
{
	if (this != &other)
	{
		_name = other._name;
		_arguments = other._arguments;
		_directives = other._directives;
		_blocks = other._blocks;
	}
	return *this;
}

BlockParser::~BlockParser()
{
}

/**
 * Recoge la cabecera y parsea el contenido hasta la llave de cierre recibida.
 */
BlockParser::BlockParser(const std::vector<Token> &tokens,
	std::vector<Token>::const_iterator iterator,
	std::vector<Token>::const_iterator closingBrace, size_t depth)
	: _name(), _arguments(), _directives(), _blocks()
{
	if (iterator == tokens.end() || closingBrace == tokens.end() ||
		iterator >= closingBrace || closingBrace->getType() != 1 ||
		closingBrace->getValue() != "}")
		throw std::runtime_error("Error: block without closing brace");
	if (iterator->getType() != 2)
		throw std::runtime_error("Error: block must start with a name");
	_name = iterator->getValue();
	++iterator;
	while (iterator != closingBrace && iterator->getType() == 2)
	{
		_arguments.push_back(iterator->getValue());
		++iterator;
	}
	if (iterator == closingBrace || iterator->getValue() != "{")
		throw std::runtime_error("Error: block without opening brace: " + _name);
	++iterator;
	parseContents(tokens, iterator, closingBrace, depth);
}

/**
 * Distingue las directivas terminadas en ; de los bloques abiertos con {.
 * Permite solo un nivel de bloques hijos dentro del bloque exterior.
 * Cada bloque hijo conserva sus directivas y no las mezcla con las del padre.
 */
void BlockParser::parseContents(const std::vector<Token> &tokens,
	std::vector<Token>::const_iterator iterator,
	std::vector<Token>::const_iterator closingBrace, size_t depth)
{
	while (iterator != closingBrace)
	{
		if (iterator->getType() != 2)
			throw std::runtime_error("Error: expected a directive or block name in " + _name);
		std::vector<Token>::const_iterator statementBegin = iterator;
		while (iterator != closingBrace && iterator->getType() == 2)
			++iterator;
		if (iterator == closingBrace)
			throw std::runtime_error("Error: missing semicolon or opening brace after " +
				statementBegin->getValue());
		if (iterator->getValue() == ";")
		{
			size_t index = static_cast<size_t>(
				std::distance(tokens.begin(), statementBegin));
			_directives.push_back(Directives(tokens, index));
		}
		else if (iterator->getValue() == "{")
		{
			if (depth >= 1)
				throw std::runtime_error("Error: nested blocks are not allowed inside " + _name);
			iterator = findClosingBrace(iterator, closingBrace);
			_blocks.push_back(BlockParser(tokens, statementBegin, iterator, depth + 1));
		}
		else
			throw std::runtime_error("Error: unexpected token in block " + _name +
				": " + iterator->getValue());
		++iterator;
	}
}

/**
 * Localiza el cierre de un bloque hijo sin rebasar el cierre de su padre.
 */
std::vector<Token>::const_iterator BlockParser::findClosingBrace(
	std::vector<Token>::const_iterator openingBrace,
	std::vector<Token>::const_iterator limit) const
{
	int braceCount = 1;
	std::vector<Token>::const_iterator iterator = openingBrace;
	++iterator;
	while (iterator != limit)
	{
		if (iterator->getType() == 1)
		{
			if (iterator->getValue() == "{")
				++braceCount;
			else if (iterator->getValue() == "}")
			{
				--braceCount;
				if (braceCount == 0)
					return iterator;
			}
		}
		++iterator;
	}
	throw std::runtime_error("Error: nested block without closing brace in " + _name);
}

const std::string &BlockParser::getName() const
{
	return _name;
}

const std::vector<std::string> &BlockParser::getArguments() const
{
	return _arguments;
}

const std::vector<Directives> &BlockParser::getDirectives() const
{
	return _directives;
}

const std::vector<BlockParser> &BlockParser::getBlocks() const
{
	return _blocks;
}
