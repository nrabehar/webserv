#include "EventManager.hpp"

EventManager::EventManager() {}
EventManager::~EventManager() {}

void EventManager::addSocket(Socket *socket)
{
	struct pollfd pfd;
	pfd.fd = socket->getFd();
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pfds.push_back(pfd);
	_sockets[pfd.fd] = socket;
}

void EventManager::addClient(int fd)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	_pfds.push_back(pfd);
	_sockets[fd] = NULL;
}

void EventManager::removeHandled(int fd)
{
	std::vector<struct pollfd>::iterator it;
	for (it = _pfds.begin(); it != _pfds.end(); ++it)
	{
		if ((*it).fd == fd)
			break;
	}
	if (it != _pfds.end())
		_pfds.erase(it);
	_sockets.erase(fd);
}

int EventManager::waitEvents()
{
	if (_pfds.empty())
		return (0);
	int n_events = poll(&_pfds[0], _pfds.size(), POLL_TIMEOUT);
	if (n_events < 0 && errno != EINTR)
		throw WException("Poll error: " + toString(strerror(errno)));
	return (n_events);
}

Socket *EventManager::getSocket(int fd)
{
	std::map<int, Socket *>::const_iterator it = _sockets.find(fd);
	if (it != _sockets.end())
		return it->second;
	return (NULL);
}

std::vector<EventInfo> EventManager::handleEvent()
{
	std::vector<EventInfo> events;

	if (waitEvents() <= 0)
		return events;

	for (size_t i = 0; i < _pfds.size(); ++i)
	{
		struct pollfd &pfd = _pfds[i];
		if (pfd.revents == 0)
			continue;

		EventInfo	event;
		event.fd = pfd.fd;
		event.events = pfd.revents;

		events.push_back(event);
		pfd.revents = 0;
	}

	return (events);
}
