/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:35:29 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 15:01:38 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"

EventManager::EventManager() {}
EventManager::~EventManager() {}

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

void EventManager::removeSocket(int fd)
{
	_pfds.erase(std::remove_if(
			_pfds.begin(), _pfds.end(),
			fdMatcher(fd)));
	_sockets.erase(fd);
}

int EventManager::wait()
{
	if (_pfds.empty())
		return (0);
	return (poll(&_pfds[0], _pfds.size(), POLL_TIMEOUT));
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

Socket *EventManager::getScoket(int fd)
{
	std::map<int, Socket *>::const_iterator it = _sockets.find(fd);
	return (it != _sockets.end() ? it->second : NULL);
}
