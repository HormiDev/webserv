/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:46 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/08/03 02:38:16 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_SERVERPARSER_HPP
#define TEST_SERVERPARSER_HPP

#include "Directives.hpp"
#include "Token.hpp"
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
	void setDirectives(const std::vector<Directives> &directives);

private:
	std::vector<Directives> _directives;
};

#endif
