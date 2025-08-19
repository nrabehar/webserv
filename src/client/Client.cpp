#include "Client.hpp"

Client::Client(int fd) : _fd(fd) {}
Client::~Client()
{
	if (_fd >= 0)
	{
		close(_fd);
		std::cout << "Client " << _fd << ": closed" << std::endl;
	}
	_fd = -1;
}

int Client::getFd() const { return _fd; }
