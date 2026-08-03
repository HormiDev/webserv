/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:54 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/07/30 06:09:57 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include "src/Token.hpp"
#include <string>
#include <vector>

class Directives
{
public:
	Directives();
	Directives(const Directives &other);
	Directives &operator=(const Directives &other);
	~Directives();
	Directives(const std::vector<Token> &tokens, size_t index);

	const std::string &getDirective() const;
	const std::vector<std::string> &getArguments() const;
	void setDirective(const std::string &directive);
	void setArguments(const std::vector<std::string> &arguments);
	void check_directive() const;

private:
	std::string _directive;
	std::vector<std::string> _arguments;

	void check_index() const;
	void check_listen() const;
	void check_listen_argument(const std::string &argument) const;
	bool is_ipv4(const std::string &argument) const;
	bool is_port(const std::string &argument) const;
	bool is_ipv4_port(const std::string &argument) const;
	void check_root() const;
	void check_error_page() const;
	void check_client_max_body_size() const;
};

#endif
