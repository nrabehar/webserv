#include "webserv.hpp"

using namespace Net;

#include "webserv.hpp"

Client::Client(int fd, Server * server)
	: EventHandler(fd), _server(server),
	_keep_alive(false), _last_active(time(NULL))
{

	LOG("Client connected: " + String::str(fd));

}

Client::~Client() {}

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

	LOG("Client read data on fd: " + String::str(_fd));

}

void	Client::onWrite()
{

	if (_out.readable() == 0)
		return ;
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
	LOG("Client write data on fd: " + String::str(_fd));

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
