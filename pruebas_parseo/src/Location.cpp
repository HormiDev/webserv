/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 12:23:58 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/30 15:29:24 by ide-dieg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

/**
 * Constructor for the Location class.
 * 
 * path: The path for the location.
 */
Location::Location(const std::string &path)
	: _path(path), _hasRoot(false), _hasIndex(false), _hasAutoindex(false),
	  _autoindex(false), _hasAllowedMethods(false), _hasRedirect(false),
	  _redirectCode(OK), _hasUploadStore(false), _hasCgi(false) {}


/**
 * Copy constructor for the Location class.
 * 
 * other: The Location object to copy from.
 */
Location::Location(const Location &other)
	: _path(other._path), _hasRoot(other._hasRoot), _root(other._root),
	  _hasIndex(other._hasIndex), _index(other._index), _hasAutoindex(other._hasAutoindex),
	  _autoindex(other._autoindex), _hasAllowedMethods(other._hasAllowedMethods),
	  _allowedMethods(other._allowedMethods),
	  _hasRedirect(other._hasRedirect), _redirectCode(other._redirectCode),
	  _redirectTarget(other._redirectTarget), _hasUploadStore(other._hasUploadStore),
	  _uploadStore(other._uploadStore), _hasCgi(other._hasCgi),
	  _cgiExtension(other._cgiExtension), _cgiExecutable(other._cgiExecutable) {}

/** * Assignment operator for the Location class.
 *
 * other: The Location object to assign from.
 * @return A reference to the assigned Location object.
 */
Location &Location::operator=(const Location &other)
{
	if (this != &other)
	{
		_path = other._path;
		_hasRoot = other._hasRoot;
		_root = other._root;
		_hasIndex = other._hasIndex;
		_index = other._index;
		_hasAutoindex = other._hasAutoindex;
		_autoindex = other._autoindex;
		_hasAllowedMethods = other._hasAllowedMethods;
		_allowedMethods = other._allowedMethods;
		_hasRedirect = other._hasRedirect;
		_redirectCode = other._redirectCode;
		_redirectTarget = other._redirectTarget;
		_hasUploadStore = other._hasUploadStore;
		_uploadStore = other._uploadStore;
		_hasCgi = other._hasCgi;
		_cgiExtension = other._cgiExtension;
		_cgiExecutable = other._cgiExecutable;
	}
	return *this;
}

/**
 * Destructor for the Location class.
 */
Location::~Location() {}

/**
 * Get the path of the location.
 * 
 * @return The path of the location.
 */
const std::string &Location::getPath() const
{
	return _path;
}

/**
 * Check if the location has a root defined.
 * 
 * @return true if the location has a root, false otherwise.
 */
bool Location::hasRoot() const
{
	return _hasRoot;
}

/**
 * Get the root of the location.
 * 
 * @return The root of the location.
 */
const std::string &Location::getRoot() const
{
	return _root;
}

/**
 * Check if the location has an index defined.
 * 
 * @return true if the location has an index, false otherwise.
 */
bool Location::hasIndex() const
{
	return _hasIndex;
}

/**
 * Get the index of the location.
 * 
 * @return The index of the location.
 */
const std::string &Location::getIndex() const
{
	return _index;
}

/**
 * Check if the location has autoindex enabled.
 * 
 * @return true if autoindex is enabled, false otherwise.
 */
bool Location::hasAutoindex() const
{
	return _hasAutoindex;
}

/**
 * Get the autoindex status of the location.
 * 
 * @return true if autoindex is enabled, false otherwise.
 */
bool Location::getAutoindex() const
{
	return _autoindex;
}

/**
 * Check if the location has allowed methods defined.
 * 
 * @return true if the location has allowed methods, false otherwise.
 */
bool Location::hasAllowedMethods() const
{
	return _hasAllowedMethods;
}

/**
 * Get the allowed HTTP methods for the location.
 * 
 * @return A set of allowed HTTP methods.
 */
const std::set<HTTPMethod> &Location::getAllowedMethods() const
{
	return _allowedMethods;
}

/**
 * Check if the location has a redirect defined.
 * 
 * @return true if the location has a redirect, false otherwise.
 */
bool Location::hasRedirect() const
{
	return _hasRedirect;
}

/**
 * Get the redirect code of the location.
 * 
 * @return The redirect code of the location.
 */
HTTPStatus Location::getRedirectCode() const
{
	return _redirectCode;
}

/**
 * Get the redirect target of the location.
 * 
 * @return The redirect target of the location.
 */
const std::string &Location::getRedirectTarget() const
{
	return _redirectTarget;
}

/**
 * Check if the location has an upload store defined.
 * 
 * @return true if the location has an upload store, false otherwise.
 */
bool Location::hasUploadStore() const
{
	return _hasUploadStore;
}

/**
 * Get the upload store path of the location.
 * 
 * @return The upload store path of the location.
 */
const std::string &Location::getUploadStore() const
{
	return _uploadStore;
}

/**
 * Check if the location has CGI defined.
 * 
 * @return true if the location has CGI, false otherwise.
 */
bool Location::hasCgi() const
{
	return _hasCgi;
}

/**
 * Get the CGI extension of the location.
 * 
 * @return The CGI extension of the location.
 */
const std::string &Location::getCgiExtension() const
{
	return _cgiExtension;
}

/**
 * Get the CGI executable of the location.
 * 
 * @return The CGI executable of the location.
 */
const std::string &Location::getCgiExecutable() const
{
	return _cgiExecutable;
}

/**
 * Set the root of the location.
 * 
 * root: The root to set for the location.
 */
void Location::setRoot(const std::string& root)
{
	_root = root;
	_hasRoot = true;
}

/**
 * Set the index of the location.
 * 
 * index: The index to set for the location.
 */
void Location::setIndex(const std::string& index)
{
	_index = index;
	_hasIndex = true;
}

/**
 * Set the autoindex status of the location.
 * 
 * value: true to enable autoindex, false to disable.
 */
void Location::setAutoindex(bool value)
{
	_autoindex = value;
	_hasAutoindex = true;
}

/**
 * Add an allowed HTTP method for the location.
 * 
 * method: The HTTP method to add to the allowed methods.
 */
void Location::addAllowedMethod(HTTPMethod method)
{
	_allowedMethods.insert(method);
	if (!_hasAllowedMethods)
		_hasAllowedMethods = true;
}

/**
 * Set the redirect for the location.
 * 
 * code: The redirect code to set for the location.
 * target: The redirect target to set for the location.
 */
void Location::setRedirect(HTTPStatus code, const std::string& target)
{
	_redirectCode = code;
	_redirectTarget = target;
	_hasRedirect = true;
}

/**
 * Set the upload store path for the location.
 *
 * path: The upload store path to set for the location.
 */
void Location::setUploadStore(const std::string& path)
{
	_uploadStore = path;
	_hasUploadStore = true;
}

/**
 * Set the CGI configuration for the location.
 * 
 * extension: The CGI extension to set for the location.
 * executable: The CGI executable to set for the location.
 */
void Location::setCgi(const std::string& extension, const std::string& executable)
{
	_cgiExtension = extension;
	_cgiExecutable = executable;
	_hasCgi = true;
}

void Location::print() const
{
	std::cout << "    Location Path: " << _path << std::endl;
	if (_hasRoot)
		std::cout << "      Root: " << _root << std::endl;
	if (_hasIndex)
		std::cout << "      Index: " << _index << std::endl;
	if (_hasAutoindex)
		std::cout << "      Autoindex: " << (_autoindex ? "ON" : "OFF") << std::endl;
	if (_hasAllowedMethods)
	{
		std::cout << "      Allowed Methods: ";
		for (std::set<HTTPMethod>::const_iterator it = _allowedMethods.begin(); it != _allowedMethods.end(); ++it)
		{
			switch (*it)
			{
				case METHOD_NONE:
					std::cout << "METHOD_NONE ";
					break;
				case GET:
					std::cout << "GET ";
					break;
				case POST:
					std::cout << "POST ";
					break;
				case DELETE:
					std::cout << "DELETE ";
					break;
			}
		}
		std::cout << std::endl;
	}
	if (_hasRedirect)
		std::cout << "      Redirect: " << _redirectCode << " " << _redirectTarget << std::endl;
	if (_hasUploadStore)
		std::cout << "      Upload Store: " << _uploadStore << std::endl;
	if (_hasCgi)
		std::cout << "      CGI: " << _cgiExtension << " -> " << _cgiExecutable << std::endl;
}
