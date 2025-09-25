#include "webserv.hpp"

EventHandler::EventHandler(): _fd(-1) {}
EventHandler::EventHandler(int fd): _fd(fd) {}
EventHandler::~EventHandler()
{
	if (_fd > -1)
		::close(_fd);
}

int EventHandler::fd() const { return (_fd); }