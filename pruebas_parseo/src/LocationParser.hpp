/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParser.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:29:18 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/10/07 16:30:56 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONPARSER_CPP
#define LOCATIONPARSER_CPP

#include "src/Directives.hpp"

class LocationParser
{
	public:
		LocationParser();
		LocationParser(const LocationParser &other);
		LocationParser &operator=(const LocationParser &other);
		~LocationParser();

	private:
		std::vector<Directives> _directives;
};

#endif