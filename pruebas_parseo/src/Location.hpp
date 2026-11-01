/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 11:21:31 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/30 15:29:53 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <iostream>
#include "http/HTTPMethods.hpp"
#include "http/HTTPStatus.hpp"
#include <set>

class Location
{
public:
	Location(const std::string &path);
	Location(const Location &other);
	Location &operator=(const Location &other);
	~Location();

	const std::string &getPath() const;

	bool hasRoot() const;
	const std::string &getRoot() const;
	bool hasIndex() const;
	const std::string &getIndex() const;
	bool hasAutoindex() const;
	bool getAutoindex() const;
	bool hasAllowedMethods() const;
	const std::set<HTTPMethod> &getAllowedMethods() const;
	bool hasRedirect() const;
	HTTPStatus getRedirectCode() const;
	const std::string &getRedirectTarget() const;
	bool hasUploadStore() const;
	const std::string &getUploadStore() const;
	bool hasCgi() const;
	const std::string &getCgiExtension() const;
	const std::string &getCgiExecutable() const;

	void setRoot(const std::string& root);
	void setIndex(const std::string& index);
	void setAutoindex(bool enabled);
	void addAllowedMethod(HTTPMethod method);
	void setRedirect(HTTPStatus code, const std::string& target);
	void setUploadStore(const std::string& path);
	void setCgi(const std::string& extension, const std::string& executable);

	void print() const;

private:
	std::string _path; //la ruta del url

	bool _hasRoot;
	std::string _root; // La ruta del equipo

	bool _hasIndex; //Indica que hacer cuando la ruta es un directorio
	std::string _index;

	bool _hasAutoindex; //Indica si se puede hacer un listado de los ficheros del directorio en caso de que no haya un _index
	bool _autoindex;

	bool _hasAllowedMethods; // NUEVO: Indica si se han definido métodos permitidos para esta location
	std::set<HTTPMethod> _allowedMethods; //los métodos permitidos para esa location.

	bool _hasRedirect;
	HTTPStatus _redirectCode; // NUEVO Cambio de tipo int (anterior) a tipo HTTPStatus (enum) para almacenar el código de redirección.
	std::string _redirectTarget;

	bool _hasUploadStore;
	std::string _uploadStore;

	bool _hasCgi;
	std::string _cgiExtension;
	std::string _cgiExecutable;
};

#endif