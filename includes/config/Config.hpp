/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 15:25:37 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/21 12:19:22 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <map>
#include "colors.hpp"

class Config
{
public:
	Config();
	Config(const Config &other);
	Config &operator=(const Config &other);
	~Config();

	int getPort() const;
	std::string getRoot() const;
	std::string getIndex() const;
	std::string getHost() const;
	std::string getErrorPage(int errorCode) const;
	void setPort(int port);
	void setRoot(const std::string &root);
	void setIndex(const std::string &index);
	void setHost(const std::string &host);
	void setErrorPage(int errorCode, const std::string &errorPagePath);

	void print() const;

private:
	std::string _root;
	std::string _index;
	std::string _host;
	int _port;
	std::map<int, std::string>
		_errorPages; // Map para almacenar las páginas de error personalizadas, donde la clave es el código de error y el valor es la ruta del archivo HTML correspondiente.
};

#endif