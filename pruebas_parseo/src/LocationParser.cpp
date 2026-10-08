/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParser.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:27:51 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/10/07 16:31:48 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/LocationParser.hpp"

LocationParser::LocationParser() {}

LocationParser::LocationParser(const LocationParser &other)
	: _directives(other._directives) {}

LocationParser &LocationParser::operator=(const LocationParser &other)
{
	if (this != &other)
		_directives = other._directives;
	return *this;
}

LocationParser::~LocationParser() {}

