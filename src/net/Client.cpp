#include "webserv.hpp"

using namespace Net;

#include "webserv.hpp"

Client::Client(int fd, Server * server)
	: EventHandler(fd), _server(server), _handler(this), _parser(&_handler),
	_keep_alive(false), _last_active(time(NULL))
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

	else if (e & POLLOUT)
		onWrite();

	else if (e & POLLHUP)
		EventLoop::instance().delHandler(this);

	else if (e & (POLLERR | POLLHUP | POLLNVAL))
		onError();

}

void	Client::onRead()
{

	if (!readSocket())
		return ;

	if (_parser.parseNext(_in, _req, _res))
		return ;
	if (_parser.state() == _parser.PS_ERROR)
		_handler.setStatus(Handler::HS_BAD_REQUEST);

	_last_active = time(NULL);
	_handler.handle(_req, _res);
	_parser.reset();

}

void	Client::onWrite()
{

	if (_handler.status() == Handler::HS_OK)
		_out.append(_res.str());
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
	else
		_out.hasRead(ret);
	if (!_out.readable())
		_out.clear();
	if (_handler.status() == Handler::HS_OK)
	{
		_keep_alive = _res.header("Connection") == "keep-alive";
		_handler.reset();
		if (_keep_alive)
			EventLoop::instance().modHandler(this, POLLIN | POLLOUT);
		else
			EventLoop::instance().delHandler(this);
		_req = Http::Request();
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
	LOG("Client " + String::str(_fd) + " receive: " + String::str(ret) + " bytes | it's buff is now: " + String::str(_in.readable()));
	return (true);

}

bool	Client::keepAlive() const { return (_keep_alive); }
void	Client::setKeepAlive(bool keep_alive) { _keep_alive = keep_alive; }
time_t	Client::lastActive() const { return (_last_active); }
Server *Client::getServer() const { return (_server); }
