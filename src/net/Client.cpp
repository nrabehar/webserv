#include "webserv.hpp"

using namespace Net;

#include "webserv.hpp"

Client::Client(int fd, Server * server)
	: EventHandler(fd), _server(server), _handler(this),
	_keep_alive(false), _last_active(time(NULL))
{

	LOG("Client connected: " + String::str(fd));

}

Client::~Client()
{
	LOG("Client disconnected: " + String::str(_fd));
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

void	Client::onRead()
{

	if (!readSocket())
		return ;
	
	if (_parser.parseNext(_in, _req))
		return ;
	
	_last_active = time(NULL);
	_handler.handle(_req, _res);
	_parser.reset();
	EventLoop::instance().modHandler(this, POLLOUT|POLLIN);

}

void	Client::onWrite()
{

	if (_handler.status() == Handler::HS_OK)
	{
		if (_res.header().find("Connection") != _res.header().end() &&
			_res.header().find("Connection")->second == "keep-alive")
			_keep_alive = true;
		else
			_keep_alive = false;
		_out.append(_res.str()); //TODO optimize this [chunked encoding or change _res.str() to build into a buffer directly]
	}
	if (_out.readable() == 0)
		return ;
	LOG(_req.method() + " " + _req.uri() + " -> " + String::str(_res.status()) + " on fd: " + String::str(_fd));
	LOG("Sending: (" + String::str(_out.readable()) + " bytes)" + std::string(_out.readPtr()));
	const char * buf = _out.readPtr();
	size_t len = _out.readable();
	ssize_t ret = ::write(_fd, buf, len);
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
	ssize_t ret = ::read(_fd, buf, cap);
	if (ret <= 0)
	{

		if (ret < 0)
			ERR("Read error on fd: " + String::str(_fd));
		EventLoop::instance().delHandler(this);
		return (false);

	}
	else
		_in.hasWritten(ret);
	
	return (true);

}

bool	Client::keepAlive() const { return (_keep_alive); }
void	Client::setKeepAlive(bool keep_alive) { _keep_alive = keep_alive; }
time_t	Client::lastActive() const { return (_last_active); }
Server *Client::getServer() const { return (_server); }