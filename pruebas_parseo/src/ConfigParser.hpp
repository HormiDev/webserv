/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 14:38:09 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/08/01 12:10:09 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "src/Config.hpp"
#include "src/ServerParser.hpp"
#include "src/Token.hpp"
#include "src/Tokenizer.hpp"
#include <string>
#include <vector>

class ConfigParser
{
public:
	ConfigParser();
	ConfigParser(const ConfigParser &other);
	ConfigParser &operator=(const ConfigParser &other);
	~ConfigParser();

	std::vector<Config> parse(const std::string &filename);
	const std::vector<Token> &getTokens() const; // debug
	const std::vector<ServerParser> &getServers() const; // debug

private:
	std::string _fileContent;
	Tokenizer _tokenizer;
	std::vector<Token> _tokens;
	std::vector<ServerParser> _servers;

	std::string readFile(const std::string &filename) const;
	void buildServers();
	void check_listen_and_servername() const;
	Listen parseListen(const std::string &argument) const;
	std::vector<Config> buildConfigs() const;
	long strToLong(const std::string &str) const;
};

#endif
