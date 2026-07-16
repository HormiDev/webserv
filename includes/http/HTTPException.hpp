/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPException.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 13:06:19 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/09 13:20:04 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPEXCEPTION_HPP
#define HTTPEXCEPTION_HPP

#include <exception>

class HTTPException : public std::exception
{
	private:
		int _statusCode;

	public:

		HTTPException(int statusCode);
		HTTPException(const HTTPException& other);
		HTTPException& operator=(const HTTPException& other);
		virtual ~HTTPException() throw();
		
		int getStatusCode() const;
};

#endif