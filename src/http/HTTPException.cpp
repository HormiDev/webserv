/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 13:16:00 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/14 14:57:24 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HTTPException.hpp"

/** Constructor for HTTPException
 * 
 * statusCode: The HTTP status code associated with the exception.
 */
HTTPException::HTTPException(int statusCode) : _statusCode(statusCode) {}

/** Copy constructor for HTTPException
 * 
 * other: The HTTPException object to copy.
 */
HTTPException::HTTPException(const HTTPException& other) : _statusCode(other._statusCode) {}

/** Assignment operator for HTTPException
 * 
 * other: The HTTPException object to assign.
 * @return A reference to the assigned object.
 */
HTTPException& HTTPException::operator=(const HTTPException& other)
{
	if (this != &other)
		_statusCode = other._statusCode;
	return *this;
}

/** Destructor for HTTPException
 */
HTTPException::~HTTPException() throw() {}

/** Returns the HTTP status code associated with the exception.
 * 
 * @return The HTTP status code.
 */
int HTTPException::getStatusCode() const
{
	return _statusCode;
}