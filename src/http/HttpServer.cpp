/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 02:50:30 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 04:52:30 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

HttpServer::HttpServer(const Config &config)
		: _config(config),
			_net_manager(NULL),
			_con_manager(NULL) {}

HttpServer::~HttpServer()
{
	stop();
	delete _net_manager;
	delete _con_manager;
}

void HttpServer::start()
{
	std::cout << "HttpServer: Starting..." << std::endl;

	_net_manager = new NetworkManager();
	_con_manager = new ConnectionManager();

	const std::vector<Server> &servers = _config.getServers();
	for (size_t i = 0; i < servers.size(); ++i)
		_net_manager->createSocket(servers[i]);

	std::cout << "HttpServer: Started successfully" << std::endl;
}

void HttpServer::stop() {}

void HttpServer::listen()
{
	std::vector<EventInfo> events = _net_manager->getEvents();

	for (size_t i = 0; i < events.size(); ++i)
	{
		const EventInfo &event = events[i];

		if (event.events & POLLIN)
			readRequest(event.fd);

		if (event.events & POLLOUT)
			writeRequest(event.fd);

		if (event.events & (POLLERR | POLLHUP | POLLNVAL))
			errorRequest(event.fd);
	}

	_net_manager->cleanup();
	_con_manager->cleanup();
}

void HttpServer::readRequest(int fd)
{
	if (_net_manager->isServer(fd))
	{
		int c_fd = _net_manager->accept(fd);
		if (c_fd > 0)
			_con_manager->connect(c_fd);
	}
	else
	{
		_net_manager->disconnect(fd);
		_con_manager->disconnect(fd);
	}
}

void HttpServer::writeRequest(int fd)
{
	_net_manager->disconnect(fd);
	_con_manager->disconnect(fd);
}

void HttpServer::errorRequest(int fd)
{
	std::cout << "Error on fd: " << fd << std::endl;
	if (!_net_manager->isServer(fd))
	{
		_net_manager->disconnect(fd);
		_con_manager->disconnect(fd);
	}
}
