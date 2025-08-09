/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:16:34 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 05:24:06 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NetworkManager.hpp"
#include <unistd.h>

NetworkManager::NetworkManager() {}
NetworkManager::~NetworkManager() {}

std::vector<EventInfo> NetworkManager::getEvents()
{
	return _event_manager.handleEvent();
}

void NetworkManager::createSocket(const Server &server)
{
	const std::vector<size_t> ports = server.getPorts();
	const std::vector<std::string> hosts = server.getHosts();
	_sockets.reserve(_sockets.size() + ports.size());
	for (size_t i = 0; i < ports.size(); ++i)
	{
		_sockets.push_back(Socket(ports[i], hosts[i], server));
		_sockets.back().setup();
		_event_manager.addSocket(&_sockets.back());
	}
}

int NetworkManager::accept(int fd)
{
	Socket *socket = _event_manager.getSocket(fd);
	if (!socket)
		return (-1);

	int c_fd = socket->acceptConnection();
	if (c_fd > 0)
		std::cout << "Accepted connection on fd " << c_fd << std::endl;
	return c_fd;
}

bool NetworkManager::isServer(int fd)
{
	return _event_manager.getSocket(fd) != NULL;
}

void NetworkManager::addClient(int fd)
{
	_event_manager.addClient(fd);
}

void NetworkManager::disconnect(int fd)
{
	_pending_closes.push_back(fd);
}

void NetworkManager::cleanup()
{
	for (size_t i = 0; i < _pending_closes.size(); ++i)
	{
		int fd = _pending_closes[i];
		_event_manager.removeHandled(fd);
	}
	_pending_closes.clear();
}
