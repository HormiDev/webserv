/* ************************************************************************** */
/*                                                                            */
/*   CgiHandler.hpp                                                          */
/*                                                                            */
/*   Este modulo lo hice autocontenido a proposito: todavia no depende de   */
/*   Config ni de HTTPRequest, para poder terminarlo e integrarlo mas       */
/*   tarde sin pisar el trabajo de mis compis en esas otras partes.         */
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
/*   Updated: 2026/07/28 00:00:00 by ismherna         ###   ########.fr       */
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

	// Guardo aqui lo minimo que necesito para lanzar un CGI, desacoplado
	// de HTTPRequest/Config. Cuando conecte esto con el resto del server,
	// simplemente relleno esta struct a partir de esas clases reales.
	struct Request
	{
		std::string method;						  // GET, POST, DELETE...
		std::string scriptPath;					  // ruta absoluta o relativa al script en disco
		std::string pathInfo;						  // parte de la URL despues del script
		std::string queryString;					  // lo que va tras el '?'
		std::string serverProtocol;				  // "HTTP/1.1"; si viene vacio asumo ese valor
		std::string remoteAddr;					  // ip del cliente
		std::string contentType;					  // Content-Type del body, si lo hay
		std::string body;							  // body completo, ya des-chunkeado si aplica
		std::map<std::string, std::string> headers; // resto de headers HTTP
	};

	CgiHandler();
	~CgiHandler();

	// Lanzo el proceso CGI. interpreterPath puede venir vacio si el script
	// es autoejecutable (shebang). Devuelvo false si el pipe o el fork fallan.
	bool start(const Request &request, const std::string &interpreterPath);

	// fd de escritura hacia el stdin del CGI (lo uso para mandar el body en POST).
	// Devuelvo -1 si el CGI no ha arrancado o ya no queda body pendiente.
	int getStdinFd() const;

	// fd de lectura del stdout del CGI.
	// Devuelvo -1 si el CGI no ha arrancado o ya se cerro (EOF alcanzado).
	int getStdoutFd() const;

	// Escribo un trozo del body pendiente al pipe de stdin.
	// OJO: solo debo llamar a esto cuando poll() marque POLLOUT en getStdinFd().
	// Cierro el fd automaticamente en cuanto ya no queda body por mandar.
	void writeToStdin();

	// Leo del pipe de stdout y acumulo en el buffer interno.
	// OJO: solo debo llamar a esto cuando poll() marque POLLIN en getStdoutFd().
	// Marco el estado como DONE en cuanto detecto EOF (read devuelve 0).
	void readFromStdout();

	// Devuelvo true cuando el CGI ha terminado de escribir (EOF) y ya
	// he recogido el proceso con waitpid.
	bool isComplete() const;

	// Devuelvo true si se ha superado el timeout configurado. En ese caso
	// mato el proceso, lo recojo y marco el estado como TIMED_OUT.
	bool checkTimeout();

	State getState() const;

	// Salida cruda del script (headers CGI + body), lista para separar
	// con splitOutput() una vez isComplete() sea true.
	const std::string &getRawOutput() const;

	// Mato el proceso si sigue vivo, lo recojo y cierro los fds que queden
	// abiertos. Es idempotente y lo llamo tambien desde el destructor.
	void terminate();

	// Separo la salida cruda del CGI en headers + body, siguiendo el
	// formato CGI estandar (cabeceras, linea en blanco, body). Si el
	// script no imprime cabeceras, trato todo el output como body.
	static bool splitOutput(const std::string &raw, std::map<std::string, std::string> &headers,
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

	// --- Helpers de start(), separados para que cada uno se entienda solo ---

	// Codigo que se ejecuta unicamente dentro del proceso hijo: redirige
	// stdin/stdout a los pipes, se cambia al directorio del script y hace
	// execve. Si algo falla, termina el proceso con _exit(1); nunca vuelve.
	void runChild(const Request &request, const std::string &interpreterPath, int inPipe[2],
				  int outPipe[2]) const;

	// Codigo que se ejecuta unicamente en el proceso padre justo despues
	// del fork: cierra los extremos de los pipes que no le corresponden,
	// pone los suyos en no bloqueante y deja el objeto listo para el poll().
	void setupParent(pid_t childPid, int inPipe[2], int outPipe[2], const Request &request);

	void closeStdin();
	void closeStdout();
	void reapChild();

	std::vector<std::string> buildEnv(const Request &request) const;
	char **envToArgv(const std::vector<std::string> &env) const;

	// No lo hago copiable: gestiona recursos del sistema (fds, pid del hijo),
	// y copiarlo llevaria a un doble cierre o a un doble kill.
	CgiHandler(const CgiHandler &other);
	CgiHandler &operator=(const CgiHandler &other);
};

#endif
