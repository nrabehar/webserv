/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:35:29 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 21:26:04 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"
#include <unistd.h>
#include <iostream>
#include <algorithm>

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

void EventManager::addClient(int fd)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	_pfds.push_back(pfd);
	_sockets[fd] = NULL;
}

void EventManager::removeSocket(int fd)
{
	_pfds.erase(std::remove_if(
									_pfds.begin(), _pfds.end(),
									fdMatcher(fd)),
							_pfds.end());
	_sockets.erase(fd);
}

int EventManager::waitEvents()
{
	if (_pfds.empty())
		return (0);
	int n_events = poll(&_pfds[0], _pfds.size(), POLL_TIMEOUT);
	if (n_events < 0)
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

void EventManager::handleEvent()
{
	std::vector<int> to_removes;
	std::vector<int> clients;

	waitEvents();
	for (size_t i = 0; i < _pfds.size(); ++i)
	{
		struct pollfd &pfd = _pfds[i];
		if (pfd.revents == 0)
			continue;
		Socket *sock = getSocket(pfd.fd);
		bool is_server = (sock != NULL);
		if (hasReadEvent(pfd.fd))
		{
			if (is_server)
			{
				std::cout << "Pollin" << std::endl;
				int client_fd = sock->acceptConnection();
				std::cout << "Accepted client: " << client_fd << std::endl;
				if (client_fd > 0)
					clients.push_back(client_fd);
				std::cout << "Added client: " << client_fd << std::endl;
			}
			else
				std::cout << "Handle client Request" << std::endl;
		}
		else if (hasWriteEvent(pfd.fd))
		{
			std::cout << "Wait for response" << std::endl;
		}
		else if (hasErrorEvent(pfd.fd))
		{
			std::cout << "Error request" << std::endl;
			if (!is_server)
				to_removes.push_back(pfd.fd);
		}
		pfd.revents = 0;
	}
	for (size_t i = 0; i < clients.size(); ++i)
		addClient(clients[i]);
	for (size_t i = 0; i < to_removes.size(); ++i)
		removeSocket(to_removes[i]);
}
