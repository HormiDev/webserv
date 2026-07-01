/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:32:42 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/07/01 13:43:43 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <iostream>
#include "colors.hpp"

int main(int argc, char **argv)
{
	Config config;
	if (argc != 2)
	{
		std::cerr << BOLD_RED << "Usage: " << argv[0] << " <config_file>" << RESET << std::endl;
		return 1;
	}
	config.load(argv[1]);
	return 0;
}