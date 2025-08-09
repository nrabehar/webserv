/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:26:07 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 04:40:01 by nrabehar         ###   ########.fr       */
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
