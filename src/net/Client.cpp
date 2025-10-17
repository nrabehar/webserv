#include "webserv.hpp"

using namespace Net;

#include "webserv.hpp"

Client::Client(int fd, Server * server)
	: EventHandler(fd), _server(server), _handler(this, &_req, &_res), _parser(&_handler),
	_keep_alive(false), _out(NULL)
{

	LOG("Client connected: " + String::str(fd));

}

Client::~Client()
{
	LOG("Client disconnected: " + String::str(_fd));
	if (_fd > -1)
		::close(_fd);
	_fd = -1;
	_req.cleanup();
	if (_out)
		delete [] _out;
}

void	Client::handle(short e)
{

	if (e & POLLHUP)
	{
		EventLoop::instance().delHandler(this);
		return ;
	}
	if (e & (POLLERR | POLLNVAL))
		return (onError());

	if (e & POLLIN)
		onRead();

	if (e & POLLOUT)
		onWrite();

}

void	Client::onTimeout()
{

	const ServerConfig & conf = _server->getConfig();

	_timeout = static_cast<long>(conf.keepalive_timeout);

	if (Time::diff(_last_active, Time::now()) < _timeout)
		return ;

	if (_handler.status() == Handler::HS_WAITING)
	{
		EventLoop::instance().delHandler(this);
	}
	else
	{
		_handler.setStatus(Handler::HS_REQUEST_TIMEOUT);
		_handler.notifyTimeout();
	}

}

void	Client::onRead()
{

	if (!readSocket())
		return ;
	if (_handler.status() == Handler::HS_WAITING)
		_handler.setStatus(Handler::HS_PROGRESS);
	if (_parser.parseNext(_in, _req, _res))
		return ;
	reloadTimeout();
	if (_parser.state() == _parser.PS_ERROR && !_handler.isError())
		_handler.setStatus(Handler::HS_BAD_REQUEST);
	if (!_handler.isError())
	{
		_last_active = time(NULL);
		if (_handler.isCgiRequest(_req))
			setLastActive(_last_active + _server->getConfig().gateway_timeout);
	}
	_handler.handle(_req, _res);
	_parser.reset();

}

void	Client::onWrite()
{

	if (!_out || ((int)_handler.status() < 200 && (int)_handler.status() >= 600))
	{
		if (_handler.isError())
			_handler.serveError(_handler.findLocation(_req.uri()), _res);
		return ;
	}
	LOG("Client " + String::str(_fd) + ": " + _req.method() + " " +
			_req.uri() + " " + String::str(_res.status()));
	ssize_t ret = ::send(_fd, _out, _out_size, MSG_DONTWAIT);
	if (ret <= 0)
	{
		EventLoop::instance().delHandler(this);
		return ;
	}
	std::memmove(_out, _out + ret, _out_size - ret);
	_out_size -= ret;
	reloadTimeout();
	if (_out_size == 0)
	{
		delete [] _out;
		_out = NULL;
		_keep_alive = _res.header("Connection").find("keep-alive") != std::string::npos;
		if (!_keep_alive)
			EventLoop::instance().delHandler(this);
		_handler.reset();
		_res = Http::Response();
	}

}

void	Client::onError()
{

	ERR("Client " + String::str(_fd) + " error");
	EventLoop::instance().delHandler(this);

}

bool	Client::readSocket()
{

	char * buf = _in.writePtr();
	size_t cap = _in.writable();
	if (cap == 0)
	{

		_in.reserve(4096);
		buf = _in.writePtr();
		cap = _in.writable();

	}
	ssize_t ret = ::recv(_fd, buf, cap, MSG_DONTWAIT);
	if (ret <= 0)
	{
		EventLoop::instance().delHandler(this);
		return (false);
	}
	_in.hasWritten(ret);
	return (true);

}

bool	Client::keepAlive() const { return (_keep_alive); }
void	Client::setKeepAlive(bool keep_alive) { _keep_alive = keep_alive; }
Server *Client::getServer() const { return (_server); }
char *	Client::out() const { return (_out); }
size_t	Client::outSize() const { return (_out_size); }
void	Client::setOut(char *out, size_t size)
{
	if (_out)
		delete [] _out;
	_out = out;
	_out_size = size;
}
