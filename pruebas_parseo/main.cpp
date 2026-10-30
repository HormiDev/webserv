/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 21:20:09 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/08/02 21:20:30 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/ConfigParser.hpp"
#include "src/colors.hpp"
#include <exception>
#include <iostream>
#include <vector>

static void printTokens(const std::vector<Token> &tokens)
{
	std::cout << BOLD_CYAN << "\nTOKENS" << RESET << std::endl;
	for (size_t i = 0; i < tokens.size(); ++i)
		std::cout << "Token[" << i << "]"
				  << " type=" << tokens[i].getType()
				  << " value=\"" << tokens[i].getValue()
				  << "\"" << std::endl;
}

static void printDirectives(const std::vector<Directives> &directives,
	const std::string &indent)
{
	for (size_t i = 0; i < directives.size(); ++i)
	{
		std::cout << indent << directives[i].getDirective();
		const std::vector<std::string> &arguments = directives[i].getArguments();
		for (size_t j = 0; j < arguments.size(); ++j)
			std::cout << " " << arguments[j];
		std::cout << ";" << std::endl;
	}
}

static void printBlocks(const std::vector<BlockParser> &blocks,
	const std::string &indent)
{
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		std::cout << indent << blocks[i].getName();
		const std::vector<std::string> &arguments = blocks[i].getArguments();
		for (size_t j = 0; j < arguments.size(); ++j)
			std::cout << " " << arguments[j];
		std::cout << " {" << std::endl;
		printDirectives(blocks[i].getDirectives(), indent + "  ");
		printBlocks(blocks[i].getBlocks(), indent + "  ");
		std::cout << indent << "}" << std::endl;
	}
}

static void printServers(const std::vector<ServerParser> &servers)
{
	std::cout << BOLD_CYAN << "\nSERVERS" << RESET << std::endl;
	for (size_t i = 0; i < servers.size(); ++i)
	{
		std::cout << BOLD_YELLOW << "Server[" << i << "]"
				  << RESET << std::endl;
		printDirectives(servers[i].getDirectives(), "  ");
		printBlocks(servers[i].getBlocks(), "  ");
	}
}

static void printConfigs(const std::vector<Config> &configs)
{
	std::cout << BOLD_CYAN << "\nCONFIGS" << RESET << std::endl;
	for (size_t i = 0; i < configs.size(); ++i)
	{
		std::cout << BOLD_YELLOW << "Config[" << i << "]"
				  << RESET << std::endl;
		configs[i].print();
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}

	try
	{
		ConfigParser configParser;
		std::vector<Config> configs = configParser.parse(argv[1]);

		printTokens(configParser.getTokens());
		printServers(configParser.getServers());
		printConfigs(configs);
	}
	catch (const std::exception &exception)
	{
		std::cerr << RED << exception.what() << RESET << std::endl;
		return 1;
	}
	return 0;
}
