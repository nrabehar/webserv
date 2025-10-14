#include "webserv.hpp"

EventHandler::EventHandler(): _fd(-1),  _timeout(30), _last_active(time(NULL)) {}
EventHandler::EventHandler(int fd): _fd(fd),  _timeout(30), _last_active(time(NULL)) {}
EventHandler::~EventHandler()
{
	if (_fd != -1)
		::close(_fd);
	_fd = -1;
}

int EventHandler::fd() const { return (_fd); }
time_t EventHandler::lastActive() const { return (_last_active); }
void EventHandler::setLastActive(time_t t) { _last_active = t; }
void EventHandler::setTimeout(int timeout)
{
	_timeout = timeout;
	_last_active = time(NULL) + timeout;
}

void EventHandler::reloadTimeout() { _last_active = time(NULL) + _timeout; }
