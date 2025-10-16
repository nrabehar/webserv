#include "webserv.hpp"

using namespace Net;

#include "webserv.hpp"

Client::Client(int fd, Server * server)
	: EventHandler(fd), _server(server), _handler(this, &_req, &_res), _parser(&_handler),
	_keep_alive(false)
{

	LOG("Client connected: " + String::str(fd));

}

Client::~Client()
{
	LOG("Client disconnected: " + String::str(_fd));
	if (_fd > -1)
		::close(_fd);
	_fd = -1;
}

void	Client::handle(short e)
{

	if (e & POLLIN)
		onRead();

	if (e & POLLOUT)
		onWrite();

	if (e & POLLHUP)
		EventLoop::instance().delHandler(this);

	if (e & (POLLERR | POLLHUP | POLLNVAL))
		onError();

}

void	Client::onTimeout()
{

	const ServerConfig & conf = _server->getConfig();

	_timeout = static_cast<long>(conf.keepalive_timeout);

	if (Time::diff(_last_active, Time::now()) < _timeout)
		return ;
	
	if (_handler.status() == Handler::HS_WAITING)
		EventLoop::instance().delHandler(this);
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

	if ((int)_handler.status() >= 200 && (int)_handler.status() < 600)
	{
		if (_handler.isError())
		{
			_handler.serveError(_handler.findLocation(_req.uri()), _res);
		}
		_out.append(_res.str());
	}
	if (_out.readable() == 0)
		return ;
	LOG("Client " + String::str(_fd) + ": " + _req.method() + " " +
			_req.uri() + " " + String::str(_res.status()));
	const char * buf = _out.readPtr();
	size_t len = _out.readable();
	ssize_t ret = ::send(_fd, buf, len, MSG_DONTWAIT);
	if (ret <= 0)
	{

		if (ret < 0)
			ERR("Write error on fd: " + String::str(_fd));
		EventLoop::instance().delHandler(this);
		return ;

	}
	_out.hasRead(ret);	
	reloadTimeout();
	if (!_out.readable())
	{
		_out.clear();
		_keep_alive = _res.header("Connection").find("keep-alive") != std::string::npos;
		if (!_keep_alive)
			EventLoop::instance().delHandler(this);
		_handler.reset();
		_res = Http::Response();
	}

}

void	Client::onError()
{

	ERR("Client error on fd: " + String::str(_fd));
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

		if (ret < 0)
			ERR("Read error on fd: " + String::str(_fd));
		EventLoop::instance().delHandler(this);
		return (false);

	}
	_in.hasWritten(ret);
	return (true);

}

bool	Client::keepAlive() const { return (_keep_alive); }
void	Client::setKeepAlive(bool keep_alive) { _keep_alive = keep_alive; }
Server *Client::getServer() const { return (_server); }
