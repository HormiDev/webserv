/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:46 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/07/30 06:09:48 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERPARSER_HPP
#define SERVERPARSER_HPP

#include "src/Directives.hpp"
#include "src/Token.hpp"
#include <string>
#include <vector>

class ServerParser
{
public:
	ServerParser();
	ServerParser(const ServerParser &other);
	ServerParser &operator=(const ServerParser &other);
	~ServerParser();
	ServerParser(const std::vector<Token> &tokens,
		std::vector<Token>::const_iterator iterator);

	const std::vector<Directives> &getDirectives() const;
	std::string getServerName() const;
	void setDirectives(const std::vector<Directives> &directives);

private:
	std::vector<Directives> _directives;
	void check_listen() const;
};

#endif
