#include "webserv.hpp"

using namespace Handler;

CgiHandler::CgiHandler(RequestHandler * handler, Http::Request * req, Http::Response *res, int timeout)
	: _pid(-1), _handler(handler), _req(req), _res(res), _stdin_h(NULL), _stdout_h(NULL), _timeout(timeout)
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
	if (_in.readable() && (int)st < 200 && (int)st > 600)
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
	if (_pid > 0)
	{
		int status;
		pid_t r = waitpid(_pid, &status, WNOHANG);
		if (r == 0)
		{
			::kill(_pid, SIGKILL);
			waitpid(_pid, &status, 0);
		}
		_pid = -1;
	}
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
	::pipe(to_child);
	::pipe(from_child);
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
		dup2(to_child[0], STDIN_FILENO);
		dup2(from_child[1], STDOUT_FILENO);
		dup2(from_child[1], STDERR_FILENO);
		::close(to_child[0]);
		::close(from_child[1]);
		_env["SCRIPT_FILENAME"] = script;
		_env["SCRIPT_NAME"] = script;
		_env["PATH_INFO"] = script;
		_env["PATH_TRANSLATED"] = script;
		char ** env = mapToCArray(_env);
		const char ** arg = getArg(bin, script);
		execve(bin.c_str(), const_cast<char **>(arg), env);
		freeCArray(env);
		EventLoop::destroy();
		exit(EXIT_FAILURE);
	}
	else
	{
		::close(to_child[0]);
		::close(from_child[1]);
		_stdin_h = new CgiStdinHandler(this, &_in, to_child[1]);
		_stdout_h = new CgiStdoutHandler(this, &_out, from_child[0]);
		_stdin_h->setTimeout(_timeout);
		_stdout_h->setTimeout(_timeout);
		_handler->addProcess(_stdin_h, POLLIN | POLLOUT);
		_handler->addProcess(_stdout_h, POLLIN | POLLOUT);
	}
}

void	CgiHandler::initEnv()
{
	_env.clear();
	_env["REQUEST_METHOD"] = _req->method();
	_env["REQUEST_URI"] = _req->uri();
	_env["QUERY_STRING"] = "";
	size_t qpos = _req->uri().find('?');
	if (qpos != std::string::npos)
		_env["QUERY_STRING"] = _req->uri().substr(qpos + 1);
	_env["CONTENT_LENGTH"] = String::str(_req->contentLength());
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

void CgiHandler::processOutput()
{

	closeOut();
	if (_handler->isError())
		return ;
	if (!parseHeaders())
		return ;
	parseBody();

}

bool	CgiHandler::parseHeaders()
{

	size_t pos = _out.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		_handler->setStatus(HS_BAD_GATEWAY);
		return (false);
	}
	std::string headers = _out.substr(0, pos + 2);
	_out.hasRead(pos + 4);
	std::istringstream stream(headers);
	std::string line;
	while (std::getline(stream, line))
	{
		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue ;
		std::string key = String::toCamelCase(String::trim(line.substr(0, colon)), '-');
		std::string value = String::trim(line.substr(colon + 1));
		if (key == "Status")
			_res->setStatus(std::atoi(value.c_str()));
		else
			_res->setHeader(key, value);
	}
	return (true);
}

bool	CgiHandler::parseBody()
{

	_res->appendBody(_out.substr());
	_res->setHeader("Content-Length", String::str(_out.readable()));
	_out.hasRead(_out.readable());
	_handler->setStatus(HS_OK);
	return (true);

}

CgiHandler::CgiHandler() :
	_pid(-1), _handler(NULL), _req(NULL), _res(NULL), _stdin_h(NULL), _stdout_h(NULL), _timeout(30)
{
}

CgiHandler::CgiHandler(const CgiHandler & src) :
	_pid(-1), _handler(NULL), _req(NULL), _res(NULL), _stdin_h(NULL), _stdout_h(NULL), _timeout(30)
{
	*this = src;
}

CgiHandler & CgiHandler::operator=(const CgiHandler & rhs)
{
	if (this != &rhs)
	{
		_pid = rhs._pid;
		_handler = rhs._handler;
		_req = rhs._req;
		_res = rhs._res;
		_env = rhs._env;
		_arg = rhs._arg;
		_in = rhs._in;
		_out = rhs._out;
		_stdin_h = rhs._stdin_h;
		_stdout_h = rhs._stdout_h;
		_timeout = rhs._timeout;
	}
	return (*this);
}

CgiHandler::CgiStdinHandler::CgiStdinHandler(CgiHandler * cgi, Buffer * in, int fd)
	: EventHandler(fd), _cgi(cgi), _in(in), _offset(0)
{
	fcntl(_fd, F_SETFL, O_NONBLOCK);
}

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
				_in->hasRead(_in->readable());
			_cgi->closeIn();
		}
	}
}

void CgiHandler::CgiStdinHandler::onTimeout()
{

	if (Time::diff(_last_active, Time::now()) < _timeout)
		return ;
	_in->hasRead(_in->readable());
	_cgi->closeIn(Handler::HS_GATEWAY_TIMEOUT);

}

CgiHandler::CgiStdoutHandler::CgiStdoutHandler(CgiHandler * cgi, Buffer * out, int fd)
	: EventHandler(fd), _cgi(cgi), _out(out), _offset(0)
{
	fcntl(_fd, F_SETFL, O_NONBLOCK);
}

CgiHandler::CgiStdoutHandler::~CgiStdoutHandler() {}

void CgiHandler::CgiStdoutHandler::handle(short e)
{
	if (e & (POLLERR | POLLNVAL))
		return (_cgi->closeOut());
	if (e & POLLIN) {
		char buf[4096];
		ssize_t n = ::read(_fd, buf, sizeof(buf));
		if (n > 0)
			_out->append(std::string(buf, n));
		else
			return (_cgi->closeOut(Handler::HS_BAD_GATEWAY));
	}
	else if (e & POLLHUP)
	{
		while (true)
		{
			char buf[4096];
			ssize_t n = ::read(_fd, buf, sizeof(buf));
			if (n > 0)
			{
				_out->append(std::string(buf, n));
				continue ;
			}
			break ;
		}
		_cgi->processOutput();
	}
}

void CgiHandler::CgiStdoutHandler::onTimeout()
{

	if (Time::diff(_last_active, Time::now()) < _timeout)
		return ;
	ERR("CgiStdoutHandler: CGI script timed out");
	_cgi->closeOut(Handler::HS_GATEWAY_TIMEOUT);

}
