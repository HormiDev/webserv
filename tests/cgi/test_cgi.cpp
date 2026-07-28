/* ************************************************************************** */
/*                                                                            */
/*   test_cgi.cpp                                                           */
/*                                                                            */
/*   Arnes de pruebas standalone para CgiHandler. Lo hice porque el modulo  */
/*   todavia no esta conectado a Server/Client, y quiero poder probarlo     */
/*   ahora mismo sin esperar a esa integracion.                             */
/*                                                                            */
/*   Simula lo que hara el server mas adelante: un unico poll() vigilando   */
/*   a la vez el stdin y el stdout del CGI, tal y como exige el subject.    */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

#include <poll.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

// Corro un CgiHandler ya arrancado hasta que termine, usando un solo
// poll() para vigilar stdin (escritura del body) y stdout (lectura de
// la respuesta), exactamente como se hara dentro del server.
static bool runUntilDone(CgiHandler &cgi)
{
	while (!cgi.isComplete())
	{
		if (cgi.checkTimeout())
		{
			std::cerr << "  -> TIMEOUT" << std::endl;
			return false;
		}

		struct pollfd fds[2];
		int nfds = 0;

		int inFd = cgi.getStdinFd();
		int outFd = cgi.getStdoutFd();

		int inIndex = -1;
		int outIndex = -1;

		if (inFd != -1)
		{
			fds[nfds].fd = inFd;
			fds[nfds].events = POLLOUT;
			fds[nfds].revents = 0;
			inIndex = nfds;
			++nfds;
		}
		if (outFd != -1)
		{
			fds[nfds].fd = outFd;
			fds[nfds].events = POLLIN;
			fds[nfds].revents = 0;
			outIndex = nfds;
			++nfds;
		}

		if (nfds == 0)
			break; // no quedan fds abiertos pero isComplete() aun no es true: raro, salgo.

		int ready = poll(fds, nfds, 1000);
		if (ready == -1)
			return false;

		if (inIndex != -1 && (fds[inIndex].revents & POLLOUT))
			cgi.writeToStdin();
		// Importante: tambien reacciono a POLLHUP. Si el script escribe todo
		// su output y termina muy rapido, el kernel puede marcar el pipe con
		// POLLHUP sin POLLIN por delante; leer igualmente es seguro (read()
		// devuelve los datos que queden y despues 0 para marcar el EOF real).
		if (outIndex != -1 && (fds[outIndex].revents & (POLLIN | POLLHUP)))
			cgi.readFromStdout();
	}
	return cgi.getState() == CgiHandler::DONE;
}

// Construyo una Request de prueba y la lanzo contra un script + interprete
// dados, muestro el resultado separado en headers/body, y comparo el body
// contra lo que espero encontrar (needle) para dar un veredicto simple.
static bool runCase(const char *label, const std::string &scriptPath,
					 const std::string &interpreter, const std::string &method,
					 const std::string &query, const std::string &body, const char *needle)
{
	std::cout << "== " << label << " ==" << std::endl;

	CgiHandler::Request req;
	req.method = method;
	req.scriptPath = scriptPath;
	req.queryString = query;
	req.body = body;
	if (!body.empty())
		req.contentType = "text/plain";

	CgiHandler cgi;
	if (!cgi.start(req, interpreter))
	{
		std::cerr << "  -> start() ha fallado (pipe/fork)" << std::endl;
		return false;
	}

	if (!runUntilDone(cgi))
	{
		std::cerr << "  -> el CGI no ha terminado bien (estado=" << cgi.getState() << ")"
				  << std::endl;
		return false;
	}

	std::map<std::string, std::string> headers;
	std::string outBody;
	CgiHandler::splitOutput(cgi.getRawOutput(), headers, outBody);

	std::cout << "  headers:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = headers.begin();
		 it != headers.end(); ++it)
		std::cout << "    " << it->first << ": " << it->second << std::endl;
	std::cout << "  body:" << std::endl << outBody << std::endl;

	bool ok = outBody.find(needle) != std::string::npos;
	std::cout << (ok ? "  [OK]" : "  [FAIL] no encuentro el contenido esperado") << std::endl;
	std::cout << std::endl;
	return ok;
}

int main()
{
	bool allOk = true;

	allOk = runCase("GET con hello.py", "./www/cgi-bin/hello.py", "/usr/bin/python3", "GET",
					 "name=Ismael", "", "Hola desde hello.py")
			&& allOk;

	allOk = runCase("POST con hello.py", "./www/cgi-bin/hello.py", "/usr/bin/python3", "POST",
					 "", "mensaje=hola-mundo", "mensaje=hola-mundo")
			&& allOk;

	allOk = runCase("GET con hello.sh", "./www/cgi-bin/hello.sh", "/bin/sh", "GET", "foo=bar",
					 "", "Hola desde hello.sh")
			&& allOk;

	std::cout << (allOk ? "Todos los casos OK." : "Algun caso ha fallado.") << std::endl;
	return allOk ? 0 : 1;
}
