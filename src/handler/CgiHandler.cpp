#include "webserv.hpp"

using namespace Handler;


CgiHandler::CgiHandler(RequestHandler * handler, Http::Request * req, Http::Response *res)
	: _pid(-1), _handler(handler), _req(req), _res(res), _stdin_h(NULL), _stdout_h(NULL)
{}


CgiHandler::~CgiHandler()
{
	if (_pid > 0)
	{
		int status;
		pid_t r = waitpid(_pid, &status, WNOHANG);
		if (r == 0)
		{
			::kill(_pid, SIGKILL);
			waitpid(_pid, &status, 0);
		}
	}
}

void CgiHandler::closeIn(Status st)
{
	if (_in.readable() && (int)st < 400 && (int)st > 600)
		return ;
	_handler->delProcess(_stdin_h);
	_stdin_h = NULL;
	_handler->setStatus(st);
}

void CgiHandler::closeOut(Status st)
{
	_handler->delProcess(_stdout_h);
	_stdin_h = NULL;
	_handler->setStatus(st);
	//TODO: parse cgi response here
}

void CgiHandler::write(const char *data, size_t len)
{
	_in.append(std::string(data, len));
}

void CgiHandler::launch(const std::string & bin, const std::string & script)
{

	initEnv();
	int to_child[2];
	int from_child[2];
	if (::pipe(to_child) == -1)
	{
		_handler->setStatus(HS_INTERNAL_SERVER_ERROR);
		return;
	}
	if (::pipe(from_child) == -1)
	{
		::close(to_child[0]);
		::close(to_child[1]);
		_handler->setStatus(HS_INTERNAL_SERVER_ERROR);
		return;
	}
	_pid = ::fork();
	if (_pid < 0)
	{
		::close(to_child[0]);
		::close(to_child[1]);
		::close(from_child[0]);
		::close(from_child[1]);
		_handler->setStatus(HS_INTERNAL_SERVER_ERROR);
		return;
	}

	if (_pid == 0)
	{
		::close(to_child[1]);
		::close(from_child[0]);
		if (dup2(to_child[0], STDIN_FILENO) == -1)
		{
			::close(to_child[1]);
			::close(from_child[1]);
			ERR("dup2 error");
			exit(EXIT_FAILURE);
		}
		::close(to_child[0]);
		if (dup2(from_child[1], STDOUT_FILENO) == -1)
		{
			::close(from_child[1]);
			ERR("dup2 error");
			exit(EXIT_FAILURE);
		}
		::close(from_child[1]);
		_env["SCRIPT_FILENAME"] = script;
		_env["SCRIPT_NAME"] = script;
		_env["PATH_INFO"] = script;
		_env["PATH_TRANSLATED"] = script;
		char ** env = mapToCArray(_env);
		const char ** arg = getArg(bin, script);
		ERR("Executing CGI: " + bin + " " + script);
		execve(bin.c_str(), const_cast<char **>(arg), env);
		freeCArray(env);
		freeCArray(const_cast<char **>(arg));
		exit(EXIT_FAILURE);
	}
	else
	{
		::close(to_child[0]);
		::close(from_child[1]);
		fcntl(to_child[1], F_SETFL, O_NONBLOCK);
		fcntl(from_child[0], F_SETFL, O_NONBLOCK);
		_stdin_h = new CgiStdinHandler(this, &_in, to_child[1]);
		_stdout_h = new CgiStdoutHandler(this, &_out, from_child[0]);
		_handler->addProcess(_stdin_h, POLLOUT);
		_handler->addProcess(_stdout_h, POLLIN);
		LOG("Started CGI process " + String::str(_pid) + " for script: " + script);
	}
}

void	CgiHandler::initEnv()
{
	_env.clear();
	_env["REQUEST_METHOD"] = _req->method();
	_env["REQUEST_URI"] = _req->uri();
	_env["SERVER_PROTOCOL"] = _req->version();
	_env["REDIRECT_STATUS"] = "200";
	for (std::map<std::string, std::string>::const_iterator it = _req->header().begin(); it != _req->header().end(); ++it)
	{
		std::string key = headerKeyToEnv(it->first);
		if (key == "HTTP_CONTENT_TYPE" || key == "HTTP_CONTENT_LENGTH")
			key = key.substr(5);
		if (!key.empty())
			_env[key] = it->second;
	}
}

const char **	CgiHandler::getArg(const std::string & bin, const std::string & script)
{
	_arg.clear();
	_arg.push_back(bin.c_str());
	_arg.push_back(script.c_str());
	_arg.push_back(NULL);
	return (_arg.data());
}

char **	CgiHandler::mapToCArray(const std::map<std::string, std::string> & m) const
{
	char ** arr = new char *[m.size() + 1];
	size_t i = 0;
	for (std::map<std::string, std::string>::const_iterator it = m.begin(); it != m.end(); ++it, ++i)
	{
		std::string entry = it->first + "=" + it->second;
		arr[i] = new char[entry.length() + 1];
		std::strcpy(arr[i], entry.c_str());
	}
	arr[i] = NULL;
	return (arr);
}

void	CgiHandler::freeCArray(char ** arr) const
{
	if (!arr)
		return ;
	for (size_t i = 0; arr[i]; ++i)
		delete[] arr[i];
	delete[] arr;
}

std::string	CgiHandler::headerKeyToEnv(const std::string & key) const
{
	std::string env_key = "HTTP_";
	for (size_t i = 0; i < key.length(); ++i)
	{
		if (key[i] == '-')
			env_key += '_';
		else
			env_key += std::toupper(key[i]);
	}
	return (env_key);
}

CgiHandler::CgiStdinHandler::CgiStdinHandler(CgiHandler * cgi, Buffer * in, int fd)
	: EventHandler(fd), _cgi(cgi), _in(in), _offset(0) {}

CgiHandler::CgiStdinHandler::~CgiStdinHandler() {}

void CgiHandler::CgiStdinHandler::handle(short e)
{
	if (e & (POLLERR | POLLNVAL))
	{
		_in->hasRead(_in->readable());
		return (_cgi->closeIn());
	}
	if (e & POLLOUT)
	{
		if (_in->readable())
		{
			ssize_t n = ::write(_fd, _in->readPtr(), _in->readable());
			if (n > 0)
				_in->hasRead(n);
			else
			{
				_in->hasRead(_in->readable());
				return (_cgi->closeIn());
			}
		}
	}
}

// --- CgiStdoutHandler Implementation ---
CgiHandler::CgiStdoutHandler::CgiStdoutHandler(CgiHandler * cgi, Buffer * out, int fd)
	: EventHandler(fd), _cgi(cgi), _out(out), _offset(0) {}

CgiHandler::CgiStdoutHandler::~CgiStdoutHandler() {}

void CgiHandler::CgiStdoutHandler::handle(short e)
{
	if (e & (POLLERR | POLLNVAL))
		return (_cgi->closeOut());
	if (e & POLLIN) {
		ssize_t n = ::read(_fd, _out->writePtr(), _out->writable());
		if (n > 0)
		{
			_out->hasWritten(n);
			// !: We can parse the CGI response here
		} else
			return (_cgi->closeOut());
	}
	else if (e & POLLHUP)
	{
		while (true)
		{
			ssize_t n = ::read(_fd, _out->writePtr(), _out->writable());
			if (n > 0)
			{
				_out->hasWritten(n);
				continue ;
			}
			break ;
		}
		_cgi->closeOut();
	}
}

