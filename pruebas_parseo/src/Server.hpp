/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:46 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/07/30 06:09:48 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_SERVER_HPP
#define TEST_SERVER_HPP

#include "src/Directives.hpp"
#include "src/Token.hpp"
#include <vector>

class Server
{
public:
	Server();
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server();
	Server(const std::vector<Token> &tokens,
		std::vector<Token>::const_iterator iterator);

	const std::vector<Directives> &getDirectives() const;
	void setDirectives(const std::vector<Directives> &directives);

private:
	std::vector<Directives> _directives;
};

#endif
