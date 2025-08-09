/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:26:07 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 07:34:39 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionManager.hpp"

ConnectionManager::ConnectionManager() {}
ConnectionManager::~ConnectionManager() {}

void ConnectionManager::connect(int c_fd)
{
	_client_manager.addClient(c_fd);
}

void ConnectionManager::disconnect(int c_fd)
{
	_pending_closes.push_back(c_fd);
}

Client *ConnectionManager::getClient(int fd)
{
	return _client_manager.getClient(fd);
}

void ConnectionManager::cleanup()
{
	for (size_t i = 0; i < _pending_closes.size(); ++i)
	{
		int fd = _pending_closes[i];
		_client_manager.removeClient(fd);
	}
	_pending_closes.clear();
}

ssize_t ConnectionManager::readClientData(int fd, char *buffer, ssize_t size)
{
	return (recv(fd, buffer, size, 0));
}

ssize_t ConnectionManager::sendClientData(int fd, const char *data, ssize_t size)
{
	return (send(fd, data, size, 0));
}