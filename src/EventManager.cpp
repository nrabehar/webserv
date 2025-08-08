/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:35:29 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 02:34:23 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"
#include <unistd.h>
#include <iostream>
#include <algorithm>

EventManager::EventManager() {}
EventManager::~EventManager() {}

void EventManager::setHandler(IEventHandler *handler) { _handler = handler; }

EventManager::fdMatcher::fdMatcher(int fd) : _target_fd(fd) {}

bool EventManager::fdMatcher::operator()(const struct pollfd &pfd) const
{
	return pfd.fd == _target_fd;
}

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

bool EventManager::hasReadEvent(int fd) const
{
	std::vector<struct pollfd>::const_iterator it;
	for (it = _pfds.begin(); it != _pfds.end(); ++it)
	{
		if (it->fd == fd)
			return (it->revents & POLLIN);
	}
	return false;
}

bool EventManager::hasWriteEvent(int fd) const
{
	std::vector<struct pollfd>::const_iterator it;
	for (it = _pfds.begin(); it != _pfds.end(); ++it)
	{
		if (it->fd == fd)
			return (it->revents & POLLOUT);
	}
	return false;
}

bool EventManager::hasErrorEvent(int fd) const
{
	std::vector<struct pollfd>::const_iterator it;
	for (it = _pfds.begin(); it != _pfds.end(); ++it)
	{
		if (it->fd == fd)
			return (it->revents & (POLLERR | POLLHUP | POLLNVAL));
	}
	return false;
}

Socket *EventManager::getSocket(int fd)
{
	std::map<int, Socket *>::const_iterator it = _sockets.find(fd);
	return (it != _sockets.end() ? it->second : NULL);
}

std::vector<EventInfo> EventManager::handleEvent()
{
	std::vector<EventInfo> events;

	if (!_handler || waitEvents() <= 0)
		return events;

	for (size_t i = 0; i < _pfds.size(); ++i)
	{
		struct pollfd &pfd = _pfds[i];
		if (pfd.revents == 0)
			continue;

		EventInfo	event;
		event.fd = pfd.fd;
		event.revents = pfd.revents;

		events.push_back(event);
		pfd.revents = 0;
	}
	return (events);
}
