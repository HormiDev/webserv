/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 15:25:37 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/03 14:04:08 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "src/Listen.hpp"
#include "src/Location.hpp"
#include "colors.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Config
{
public:
	Config();
	Config(const Config &other);
	Config &operator=(const Config &other);
	~Config();

	std::string getRoot() const;
	std::string getIndex() const;
	const std::vector<Listen> &getListens() const;
	const std::vector<Location> &getLocations() const;
	std::string getServerName() const;
	std::string getErrorPage(int errorCode) const;
	size_t getClientMaxBodySize() const;
	void setRoot(const std::string &root);
	void setIndex(const std::string &index);
	void addListen(const Listen &listen);
	void addLocation(const Location &location);
	void setServerName(const std::string &serverName);
	void setErrorPage(int errorCode, const std::string &errorPagePath);
	void setClientMaxBodySize(size_t clientMaxBodySize);
	const Location* findLocation(const std::string &path) const;

	void print() const;

private:
	std::string _root;
	std::string _index;
	std::string _server_name;
	std::vector<Listen> _listens;
	std::map<int, std::string> _errorPages; // Map para almacenar las páginas de error personalizadas, donde la clave es el código de error y el valor es la ruta del archivo HTML correspondiente.
	size_t 	_clientMaxBodySize;
	std::vector<Location>		_locations;
};

#endif
