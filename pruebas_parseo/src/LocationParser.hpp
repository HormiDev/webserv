/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParser.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:29:18 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/10/30 15:59:38 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONPARSER_HPP
#define LOCATIONPARSER_HPP

#include "src/BlockParser.hpp"
#include "src/Directives.hpp"
#include "src/Location.hpp"

class LocationParser
{
	public:
		LocationParser();
		explicit LocationParser(const BlockParser &block);
		LocationParser(const LocationParser &other);
		LocationParser &operator=(const LocationParser &other);
		~LocationParser();

		static Location parse(const BlockParser &block);

	private:
		std::vector<Directives> _directives;
		void check_directive_types() const;
};

#endif
