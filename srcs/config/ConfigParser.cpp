/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:01:12 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/14 16:05:51 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.hpp"
#include "config/ConfigParser.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>

/**
 * Default constructor for the ConfigParser class. Initializes default value for
 * pos
 * 
 * pos: Default position index (0).
 */
ConfigParser::ConfigParser() : _pos(0)
{
	std::cout << BOLD_GREEN << "ConfigParser default constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the ConfigParser class.
 * 
 * other: The ConfigParser object to copy from.
 */
ConfigParser::ConfigParser(const ConfigParser &other) : _tokens(other._tokens), _pos(other._pos)
{
	std::cout << BOLD_GREEN << "ConfigParser copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the ConfigParser class.
 * 
 * other: The ConfigParser object to assign from.
 * @return A reference to the assigned ConfigParser object.
 */
ConfigParser &ConfigParser::operator=(const ConfigParser &other)
{
	if (this != &other)
	{
		_tokens = other._tokens;
		_pos = other._pos;
	}
	std::cout << BOLD_GREEN << "ConfigParser assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the ConfigParser class.
 */
ConfigParser::~ConfigParser()
{
	std::cout << BOLD_GREEN << "ConfigParser destructor called" << RESET << std::endl;
}

/**
 * Parses the configuration file and returns a Config object.
 * 
 * filename: The path to the configuration file.
 * @return A Config object initialized with the parsed values.
 */
Config ConfigParser::parse(const std::string &filename)
{
	Config config;

	_tokens
		.clear(); // Hacemos Clear pare eliminar cualquier token previo en caso de que se llame a parse varias veces con diferentes archivos de configuración.
	_pos = 0;

	std::ifstream configFile(filename.c_str());
	if (!configFile.is_open())
		throw std::runtime_error("Error opening config file: " + filename);

	std::stringstream buffer;
	buffer << configFile.rdbuf(); //Leemos todo el contenido del archivo en un stringstream
	std::string fileContent = buffer.str();
	std::string normalizedContent;
	//En este bucle for se normaliza el contenido de filecontent (se agregan espacios alrededor de los tokens especiales
	//  "{"  "}" y ";" para que puedan ser tokenizados correctamente con el stringstream y el operador >>)
	// Se normalizan porque podria aparecer tal que asi: "server{ port 8080;}" y eso no se tokenizaria correctamente, ya que el operador >> separa por espacios."
	// Al final lo que queremos es que esten tal que asi: "server { port 8080 ; }"" para que se tokenice correctamente.
	for (size_t i = 0; i < fileContent.size(); ++i)
	{
		if (fileContent[i] == '{' || fileContent[i] == '}' ||
			fileContent[i] == ';') // Remove carriage return characters
		{
			normalizedContent += " ";
			normalizedContent += fileContent[i];
			normalizedContent += " ";
		}
		else
			normalizedContent += fileContent[i];
	}
	//Una vez los tenemos normalizados, los tokenizamos y los guardamos en el vector _tokens. Quedaría tal que asi: server, {, port, 8080, ;, },
	std::stringstream ss(normalizedContent);
	std::string token;
	while (
		ss >>
		token) //Vamos leyento token a token con el operador >> y los vamos guardando en el vector _tokens con push_back.
		_tokens.push_back(token);

	parseServer(config);
	return config;
}

/**
 * Returns the current token.
 * 
 * @return A reference to the current token.
 */
const std::string &ConfigParser::current() const
{
	if (_pos < _tokens.size())
		return _tokens[_pos];
	throw std::out_of_range("Current position is out of range");
}

/**
 * Moves to the next token.
 * 
 * @return A reference to the next token.
 */
void ConfigParser::next()
{
	if (_pos >= _tokens.size())
		throw std::out_of_range("Next position is out of range");
	++_pos;
}

/**
 * Expects a specific token and moves to the next token.
 * If the expected token is not found, throws a runtime error.
 * 
 * token: The token to expect.
 */
void ConfigParser::expect(const std::string &token)
{
	if (current() != token)
		throw std::runtime_error("Expected token: " + token + ", but found: " + current());
	next();
}

/**
 * Parses the server block.
 * 
 * config: The Config object to initialize.
 */
void ConfigParser::parseServer(Config &config)
{
	expect(
		"server"); // Si el primer token no es "server", lanzamos un error porque sabemos que el archivo de conf esta mal formado. Si lo es, avanza al siguiente.
	expect(
		"{"); //Lo mismo qon la llave de apertura del bloque de configuracion del server. De esta manera vamos comprobando que este bien formado el archivo de configuracion.
	while (
		current() !=
		"}") // Mientras no encontremos la llave de cierre del bloque de configuracion del server, seguimos parseando los tokens.
	{
		if (current() ==
			"port") // Si el token es port parseamos el puerto y lo guardamos en el objeto config.
			parsePort(config);
		else if (current() == "root")
			parseRoot(config);
		else if (current() == "index")
			parseIndex(config);
		else
			throw std::runtime_error("Unexpected token in server block: " + current());
	}
	expect("}");
}

/**
 * Parses the port directive.
 * 
 * config: The Config object to initialize.
 */
void ConfigParser::parsePort(Config &config)
{
	expect(
		"port"); // Si el token actual es "port", avanzamos al siguiente token que debería ser el número de puerto.
	char *end;
	long port = std::strtol(
		current().c_str(), &end,
		10); // Convertimos el token actual a un número entero (long) usando strtol. end apunta al primer caracter que no se pudo convertir. (el 10 es la base a lo que lo queremos convertir)
	if (*end != '\0' || port < 1 ||
		port >
			65535) // Si end no apunta al final de la cadena, significa que hubo caracteres no numéricos en el token. Además, comprobamos que el puerto esté en el rango válido (1-65535).
		throw std::runtime_error("Invalid port number: " + current());
	config.setPort(port); // Si todo es correcto, establecemos el puerto en el objeto config.
	next();				  // Avanzamos al siguiente token, que debería ser el punto y coma.
	expect(
		";"); // Comprobamos que el siguiente token sea un punto y coma, que indica el final de la directiva de puerto. SI no lo es lanzamos un error porque no estaria bien formado.
}

/**
 * Parses the root directive.
 * 
 * config: The Config object to initialize.
 */
void ConfigParser::parseRoot(Config &config)
{
	expect(
		"root"); // Si el token actual es "root", avanzamos al siguiente token que debería ser la ruta raíz.
	std::string root = current(); // Obtenemos el valor del token actual.
	config.setRoot(root);		  // Establecemos la ruta raíz en el objeto config.
	next();						  // Avanzamos al siguiente token, que debería ser el punto y coma.
	expect(
		";"); // Comprobamos que el siguiente token sea un punto y coma, que indica el final de la directiva de raíz. SI no lo es lanzamos un error porque no estaria bien formado.
}

/**
 * Parses the index directive.
 * 
 * config: The Config object to initialize.
 */
void ConfigParser::parseIndex(Config &config)
{
	expect(
		"index"); // Si el token actual es "index", avanzamos al siguiente token que debería ser el nombre del archivo index.

	std::string index = current(); // Obtenemos el valor del token actual.
	config.setIndex(index);		   // Establecemos el nombre del archivo index en el objeto config.
	next(); // Avanzamos al siguiente token, que debería ser el punto y coma.
	expect(
		";"); // Comprobamos que el siguiente token sea un punto y coma, que indica el final de la directiva de index. SI no lo es lanzamos un error porque no estaria bien formado.
}