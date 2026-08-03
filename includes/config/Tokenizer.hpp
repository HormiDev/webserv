/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:25 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/08/03 02:36:20 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "Token.hpp"
#include <string>
#include <vector>

class Tokenizer
{
public:
	Tokenizer();
	Tokenizer(const Tokenizer &other);
	Tokenizer &operator=(const Tokenizer &other);
	~Tokenizer();

	void tokenize(const std::string &fileContent);
	void printTokens() const;
	const std::vector<Token> &getTokens() const;

private:
	std::vector<Token> _tokens;

	void check_tokens() const;
	void check_braces() const;
	void check_tokens_outside_braces() const;
	void check_server_opening_brace() const;
	void check_semicolon_has_string() const;
	bool is_special_char(char c) const;
	void splitTokens(const std::string &fileContent);
	void compressTokens();
	void skipComment(size_t &position) const;
	bool isWhitespace(const Token &token) const;
	void compressDoubleQuotedString(size_t &position,
		std::vector<Token> &compressedTokens) const;
	void compressSingleQuotedString(size_t &position,
		std::vector<Token> &compressedTokens) const;
};

#endif
