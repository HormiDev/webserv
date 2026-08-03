/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:37 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/07/30 06:09:39 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_TOKEN_HPP
#define TEST_TOKEN_HPP

#include <string>

class Token
{
public:
	Token();
	Token(const Token &other);
	Token &operator=(const Token &other);
	~Token();
	Token(const std::string &value, int type);

	const std::string &getValue() const;
	int getType() const;
	void setValue(const std::string &value);
	void setType(int type);

private:
	std::string _value;
	int _type;
};

#endif
