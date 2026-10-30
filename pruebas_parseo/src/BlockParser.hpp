#ifndef BLOCKPARSER_HPP
#define BLOCKPARSER_HPP

#include "src/Directives.hpp"
#include "src/Token.hpp"
#include <string>
#include <vector>

class BlockParser
{
public:
	BlockParser();
	BlockParser(const BlockParser &other);
	BlockParser &operator=(const BlockParser &other);
	~BlockParser();
	BlockParser(const std::vector<Token> &tokens,
		std::vector<Token>::const_iterator iterator,
		std::vector<Token>::const_iterator closingBrace);

	const std::string &getName() const;
	const std::vector<std::string> &getArguments() const;
	const std::vector<Directives> &getDirectives() const;
	const std::vector<BlockParser> &getBlocks() const;

private:
	std::string _name;
	std::vector<std::string> _arguments;
	std::vector<Directives> _directives;
	std::vector<BlockParser> _blocks;

	void parseContents(const std::vector<Token> &tokens,
		std::vector<Token>::const_iterator iterator,
		std::vector<Token>::const_iterator closingBrace);
	std::vector<Token>::const_iterator findClosingBrace(
		std::vector<Token>::const_iterator openingBrace,
		std::vector<Token>::const_iterator limit) const;
};

#endif
