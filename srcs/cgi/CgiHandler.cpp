#include "CgiHandler.hpp"

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sstream>
#include <cctype>
#include <cstdlib>

CgiHandler::CgiHandler()
	: _state(NOT_STARTED), _pid(-1), _stdinFd(-1), _stdoutFd(-1), _bodyBytesSent(0), _startTime(0)
{
}

CgiHandler::~CgiHandler()
{
	terminate();
}

/**
 * Lanzo el proceso CGI: creo los dos pipes y hago fork. Todo lo que pasa
 * dentro de cada rama del fork lo delego en runChild()/setupParent() para
 * no tener una funcion gigante donde cuesta ver que hace cada rama.
 */
bool CgiHandler::start(const Request &request, const std::string &interpreterPath)
{
	int inPipe[2];	// padre escribe -> hijo lee (stdin del hijo)
	int outPipe[2]; // hijo escribe -> padre lee (stdout del hijo)

	if (pipe(inPipe) == -1)
		return false;
	if (pipe(outPipe) == -1)
	{
		close(inPipe[0]);
		close(inPipe[1]);
		return false;
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		close(inPipe[0]);
		close(inPipe[1]);
		close(outPipe[0]);
		close(outPipe[1]);
		return false;
	}

	if (pid == 0)
	{
		runChild(request, interpreterPath, inPipe, outPipe);
		// runChild() nunca vuelve: o hace execve, o termina con _exit(1).
	}

	setupParent(pid, inPipe, outPipe, request);
	return true;
}

/**
 * Esto solo se ejecuta en el hijo. Redirijo stdin/stdout a los pipes,
 * cierro los fds que ya no necesito, me cambio al directorio del script
 * (el subject lo exige, para que los paths relativos del script funcionen)
 * y ejecuto el interprete (o el script directamente si tiene shebang).
 */
void CgiHandler::runChild(const Request &request, const std::string &interpreterPath,
						  int inPipe[2], int outPipe[2]) const
{
	dup2(inPipe[0], STDIN_FILENO);
	dup2(outPipe[1], STDOUT_FILENO);
	close(inPipe[0]);
	close(inPipe[1]);
	close(outPipe[0]);
	close(outPipe[1]);

	std::string scriptDir = ".";
	std::string scriptFile = request.scriptPath;
	size_t slash = request.scriptPath.find_last_of('/');
	if (slash != std::string::npos)
	{
		scriptDir = request.scriptPath.substr(0, slash);
		scriptFile = request.scriptPath.substr(slash + 1);
	}
	if (chdir(scriptDir.c_str()) == -1)
		_exit(1);

	std::vector<std::string> envStrings = buildEnv(request);
	char **envp = envToArgv(envStrings);

	std::string relativeScript = "./" + scriptFile;
	std::string execPath = interpreterPath.empty() ? relativeScript : interpreterPath;

	char *argv[3];
	argv[0] = const_cast<char *>(execPath.c_str());
	if (interpreterPath.empty())
	{
		argv[1] = NULL;
	}
	else
	{
		argv[1] = const_cast<char *>(relativeScript.c_str());
		argv[2] = NULL;
	}

	execve(execPath.c_str(), argv, envp);
	// Si llego aqui, execve ha fallado (script inexistente, sin permisos,
	// interprete no encontrado...). No hay vuelta atras posible.
	_exit(1);
}

/**
 * Esto solo se ejecuta en el padre, justo despues del fork. Cierro los
 * extremos de los pipes que le pertenecen al hijo, dejo los mios en modo
 * no bloqueante (asi lo exige el subject para cualquier fd que pase por
 * poll()) y dejo el objeto listo para que el server lo integre en su loop.
 */
void CgiHandler::setupParent(pid_t childPid, int inPipe[2], int outPipe[2], const Request &request)
{
	close(inPipe[0]);
	close(outPipe[1]);

	fcntl(inPipe[1], F_SETFL, O_NONBLOCK);
	fcntl(outPipe[0], F_SETFL, O_NONBLOCK);

	_pid = childPid;
	_stdinFd = inPipe[1];
	_stdoutFd = outPipe[0];
	_pendingBody = request.body;
	_bodyBytesSent = 0;
	_state = RUNNING;
	_startTime = time(NULL);

	// Si no hay body que mandar (GET, DELETE...), cierro stdin ya para que
	// el CGI vea EOF inmediatamente en su primera lectura.
	if (_pendingBody.empty())
		closeStdin();
}

int CgiHandler::getStdinFd() const
{
	return _stdinFd;
}

int CgiHandler::getStdoutFd() const
{
	return _stdoutFd;
}

/**
 * Escribo un trozo del body pendiente. Solo debo llamar a esto tras
 * recibir POLLOUT del poll() principal sobre getStdinFd().
 */
void CgiHandler::writeToStdin()
{
	if (_stdinFd == -1)
		return;

	size_t remaining = _pendingBody.size() - _bodyBytesSent;
	if (remaining == 0)
	{
		closeStdin();
		return;
	}

	ssize_t n = write(_stdinFd, _pendingBody.c_str() + _bodyBytesSent, remaining);
	if (n > 0)
		_bodyBytesSent += static_cast<size_t>(n);

	if (_bodyBytesSent == _pendingBody.size())
		closeStdin();

	// Si n <= 0 (EAGAIN u otro caso), no hago nada: el subject prohibe
	// mirar errno tras un write, asi que simplemente espero al siguiente
	// POLLOUT para reintentar.
}

/**
 * Leo del pipe de salida. Solo debo llamar a esto tras recibir POLLIN
 * del poll() principal sobre getStdoutFd().
 */
void CgiHandler::readFromStdout()
{
	if (_stdoutFd == -1)
		return;

	char buffer[4096];
	ssize_t n = read(_stdoutFd, buffer, sizeof(buffer));

	if (n > 0)
	{
		_rawOutput.append(buffer, static_cast<size_t>(n));
	}
	else if (n == 0)
	{
		// EOF real: el script ha terminado de escribir. Segun el subject,
		// si el CGI no manda Content-Length, este EOF es lo que marca el
		// final de la respuesta.
		closeStdout();
		reapChild();
		_state = DONE;
	}
	// n < 0: nada listo todavia, reintento en el siguiente POLLIN.
}

bool CgiHandler::isComplete() const
{
	return _state == DONE;
}

bool CgiHandler::checkTimeout()
{
	if (_state != RUNNING)
		return false;
	if (time(NULL) - _startTime >= TIMEOUT_SECONDS)
	{
		terminate();
		_state = TIMED_OUT;
		return true;
	}
	return false;
}

CgiHandler::State CgiHandler::getState() const
{
	return _state;
}

const std::string &CgiHandler::getRawOutput() const
{
	return _rawOutput;
}

/**
 * Mato el proceso si sigue vivo, lo recojo con waitpid para no dejar
 * zombies, y cierro cualquier fd que siga abierto. Es idempotente: puedo
 * llamarlo varias veces sin problema (por eso tambien lo llamo desde el
 * destructor, por si el caller se olvida).
 */
void CgiHandler::terminate()
{
	if (_pid > 0)
	{
		kill(_pid, SIGKILL);
		reapChild();
	}
	closeStdin();
	closeStdout();
}

void CgiHandler::closeStdin()
{
	if (_stdinFd != -1)
	{
		close(_stdinFd);
		_stdinFd = -1;
	}
}

void CgiHandler::closeStdout()
{
	if (_stdoutFd != -1)
	{
		close(_stdoutFd);
		_stdoutFd = -1;
	}
}

void CgiHandler::reapChild()
{
	if (_pid > 0)
	{
		int status;
		waitpid(_pid, &status, 0);
		_pid = -1;
	}
}

/**
 * Construyo las variables de entorno CGI/1.1 a partir de la request.
 * Los headers HTTP normales los traduzco a HTTP_NOMBRE_DE_HEADER, que es
 * el formato estandar que cualquier script CGI espera leer.
 */
std::vector<std::string> CgiHandler::buildEnv(const Request &request) const
{
	std::vector<std::string> env;

	env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env.push_back("SERVER_PROTOCOL=" + (request.serverProtocol.empty() ? std::string("HTTP/1.1")
																		: request.serverProtocol));
	env.push_back("REQUEST_METHOD=" + request.method);
	env.push_back("SCRIPT_NAME=" + request.scriptPath);
	env.push_back("PATH_INFO=" + request.pathInfo);
	env.push_back("QUERY_STRING=" + request.queryString);
	env.push_back("REMOTE_ADDR=" + request.remoteAddr);
	env.push_back("SERVER_SOFTWARE=webserv/1.0");

	if (!request.body.empty())
	{
		std::ostringstream lenStream;
		lenStream << request.body.size();
		env.push_back("CONTENT_LENGTH=" + lenStream.str());
	}
	if (!request.contentType.empty())
		env.push_back("CONTENT_TYPE=" + request.contentType);

	for (std::map<std::string, std::string>::const_iterator it = request.headers.begin();
		 it != request.headers.end(); ++it)
	{
		std::string key = "HTTP_" + it->first;
		for (size_t i = 0; i < key.size(); ++i)
		{
			if (key[i] == '-')
				key[i] = '_';
			else
				key[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(key[i])));
		}
		env.push_back(key + "=" + it->second);
	}

	return env;
}

/**
 * Convierto el vector de strings "CLAVE=valor" a un char** terminado en
 * NULL, tal como lo espera execve. Solo lo uso justo antes de execve en
 * el proceso hijo, asi que no hace falta liberar esta memoria: o el
 * proceso reemplaza su imagen, o termina con _exit.
 */
char **CgiHandler::envToArgv(const std::vector<std::string> &env) const
{
	char **argv = new char *[env.size() + 1];
	for (size_t i = 0; i < env.size(); ++i)
		argv[i] = const_cast<char *>(env[i].c_str());
	argv[env.size()] = NULL;
	return argv;
}

/**
 * Separo la salida cruda del CGI en headers + body. El formato CGI
 * estandar es: cabeceras tipo "Clave: valor", una linea en blanco, y el
 * body. Si el script no imprime ninguna cabecera, trato todo como body
 * (esto es valido segun el subject).
 */
bool CgiHandler::splitOutput(const std::string &raw, std::map<std::string, std::string> &headers,
							 std::string &body)
{
	size_t headerEnd = raw.find("\r\n\r\n");
	size_t separatorLen = 4;

	if (headerEnd == std::string::npos)
	{
		headerEnd = raw.find("\n\n");
		separatorLen = 2;
		if (headerEnd == std::string::npos)
		{
			body = raw;
			return true;
		}
	}

	std::string headerBlock = raw.substr(0, headerEnd);
	body = raw.substr(headerEnd + separatorLen);

	std::istringstream stream(headerBlock);
	std::string line;
	while (std::getline(stream, line))
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue;

		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 1);
		size_t start = value.find_first_not_of(' ');
		if (start != std::string::npos)
			value = value.substr(start);

		headers[key] = value;
	}

	return true;
}
