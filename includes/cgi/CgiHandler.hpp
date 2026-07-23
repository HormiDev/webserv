/* ************************************************************************** */
/*                                                                            */
/*   CgiHandler.hpp                                                          */
/*                                                                            */
/*   Modulo CGI autocontenido. No depende de Config ni de HTTPRequest        */
/*   actuales a proposito, para poder integrarse mas tarde sin conflictos.   */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ismherna <ismherna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 15:25:37 by ismherna          #+#    #+#             */
/*   Updated: 2026/07/21 12:19:22 by ismherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <sys/types.h>

class CgiHandler
{
public:
	enum State
	{
		NOT_STARTED,
		RUNNING,
		DONE,
		FAILED,
		TIMED_OUT
	};

	// Datos minimos que el CGI necesita, desacoplados de HTTPRequest/Config.
	// Cuando se integre, esto se rellena a partir de esas clases reales.
	struct Request
	{
		std::string method;         // GET, POST, DELETE...
		std::string scriptPath;     // ruta absoluta o relativa al script en disco
		std::string pathInfo;       // parte de la URL despues del script
		std::string queryString;    // lo que va tras el '?'
		std::string serverProtocol; // "HTTP/1.1" si vacio se asume ese valor
		std::string remoteAddr;     // ip del cliente
		std::string contentType;    // Content-Type del body, si lo hay
		std::string body;           // body completo, ya des-chunkeado si aplica
		std::map<std::string, std::string> headers; // resto de headers HTTP
	};

	CgiHandler();
	~CgiHandler();

	// Lanza el proceso CGI. interpreterPath puede ir vacio si el script
	// es autoejecutable (shebang). Devuelve false si fork/pipe fallan.
	bool start(const Request &request, const std::string &interpreterPath);

	// fd de escritura hacia el stdin del CGI (para mandar el body en POST).
	// -1 si el CGI no ha arrancado o ya no queda body pendiente.
	int getStdinFd() const;

	// fd de lectura del stdout del CGI.
	// -1 si el CGI no ha arrancado o ya se cerro (EOF alcanzado).
	int getStdoutFd() const;

	// Escribe un trozo del body pendiente al pipe de stdin.
	// Llamar SOLO cuando poll() marque POLLOUT en getStdinFd().
	// Cierra el fd automaticamente cuando ya no queda body por mandar.
	void writeToStdin();

	// Lee del pipe de stdout y acumula en el buffer interno.
	// Llamar SOLO cuando poll() marque POLLIN en getStdoutFd().
	// Marca el estado como DONE cuando detecta EOF (read devuelve 0).
	void readFromStdout();

	// true cuando el CGI ha terminado de escribir (EOF) y se ha
	// recogido el proceso con waitpid.
	bool isComplete() const;

	// true si se ha superado el timeout configurado; en ese caso mata
	// el proceso, lo recoge y marca el estado como TIMED_OUT.
	bool checkTimeout();

	State getState() const;

	// Salida cruda del script (headers CGI + body), lista para separar
	// con splitOutput() una vez isComplete() sea true.
	const std::string &getRawOutput() const;

	// Mata el proceso si sigue vivo, lo recoge y cierra los fds abiertos.
	// Se llama automaticamente desde el destructor.
	void terminate();

	// Separa la salida cruda del CGI en headers + body, siguiendo el
	// formato CGI estandar (headers, linea en blanco, body). Si el
	// script no imprime headers, todo el output se trata como body.
	static bool splitOutput(const std::string &raw,
							 std::map<std::string, std::string> &headers,
							 std::string &body);

private:
	State _state;
	pid_t _pid;
	int _stdinFd;  // extremo de escritura, padre -> hijo (stdin del CGI)
	int _stdoutFd; // extremo de lectura, hijo -> padre (stdout del CGI)

	std::string _pendingBody;
	size_t _bodyBytesSent;
	std::string _rawOutput;

	time_t _startTime;
	static const int TIMEOUT_SECONDS = 10;

	void closeStdin();
	void closeStdout();
	void reapChild();

	std::vector<std::string> buildEnv(const Request &request) const;
	char **envToArgv(const std::vector<std::string> &env) const;

	// No copiable: gestiona recursos del sistema (fds, pid del hijo).
	CgiHandler(const CgiHandler &other);
	CgiHandler &operator=(const CgiHandler &other);
};

#endif
