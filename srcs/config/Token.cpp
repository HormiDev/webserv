/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 06:09:41 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/08/03 02:35:13 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Token.hpp"

Token::Token() : _value(""), _type(0)
{
}

Token::Token(const Token &other) : _value(other._value), _type(other._type)
{
}

Token &Token::operator=(const Token &other)
{
	if (this != &other)
	{
		_value = other._value;
		_type = other._type;
	}
	return *this;
}

Token::~Token()
{
}

Token::Token(const std::string &value, int type)
	: _value(value), _type(type)
{
}

const std::string &Token::getValue() const
{
	return _value;
}

int Token::getType() const
{
	return _type;
}

void Token::setValue(const std::string &value)
{
	_value = value;
}

void Token::setType(int type)
{
	_type = type;
}
