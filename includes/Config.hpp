/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:00:54 by ide-dieg          #+#    #+#             */
/*   Updated: 2026/07/01 13:45:30 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include "../includes/colors.hpp"

class Config
{
public:
	Config();
	Config(const Config &other);
	Config &operator=(const Config &other);
	~Config();
	Config(const std::string &filename);

	void load(const std::string &filename);
	int getPort() const;
	std::string getRoot() const;
	std::string getIndex() const;
	void setPort(int port);
	void setRoot(const std::string &root);
	void setIndex(const std::string &index);

private:
	std::string _filename;
	std::string _root;
	std::string _index;
	int _port;
};

#endif