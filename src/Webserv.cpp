/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:35:15 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 02:35:14 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

WebServ::~WebServ() {}

WebServ::WebServ(const std::string &configPath) : _conf(configPath), _running(false)
{
	_conf.validate();
	_conf.printConfig();
	_event_manager.setHandler(this);
}

void WebServ::createSocket(const Server &server)
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

void WebServ::cleanupClient(int fd)
{
	std::cout << "Cleaning up client " << fd << std::endl;
	_event_manager.removeHandled(fd);
	_client_manager.removeClient(fd);
}

void WebServ::processEvents(const std::vector<EventInfo> &events)
{
	for (size_t i = 0; i < events.size(); ++i)
	{
		const EventInfo &event = events[i];
		if (event.revents & POLLIN)
			onRead(event.fd);
		if (event.revents & POLLOUT)
			onWrite(event.fd);
		if (event.revents & (POLLERR | POLLHUP | POLLNVAL))
			onError(event.fd);
	}
	std::vector<int>::const_iterator it;
	for (it = _new_clients.begin(); it != _new_clients.end(); ++it)
	{
		_client_manager.addClient(*it);
		_event_manager.addClient(*it);
	}
	for (it = _client_to_removes.begin(); it != _client_to_removes.end(); ++it)
	{
		_event_manager.removeHandled(*it);
		_client_manager.removeClient(*it);
	}
	_new_clients.clear();
	_client_to_removes.clear();
}

void WebServ::run()
{
	const std::vector<Server> &servers = _conf.getServers();
	std::vector<Server>::const_iterator it;
	for (it = servers.begin(); it != servers.end(); ++it)
		createSocket(*it);
	for (; !Signal::shouldStop();)
	{
		std::vector<EventInfo> events = _event_manager.handleEvent();
		processEvents(events);
	}
}

void WebServ::onRead(int fd)
{
	Socket *socket = _event_manager.getSocket(fd);
	if (socket)
	{
		int client_fd = socket->acceptConnection();
		if (client_fd > 0)
		{
			_new_clients.push_back(client_fd);
			std::cout << "New client: " << client_fd << std::endl;
		}
	}
	else
	{
		Client *client = _client_manager.getClient(fd);
		if (!client)
			return;
		_client_to_removes.push_back(fd);
	}
}

void WebServ::onWrite(int fd)
{
	std::cout << "Client " << fd << " ready for write" << std::endl;
	_client_to_removes.push_back(fd);
}

void WebServ::onError(int fd)
{
	std::cout << "Error on fd " << fd << std::endl;
	if (!_event_manager.getSocket(fd))
		_client_to_removes.push_back(fd);
}

int main(int ac, char **av)
{
	if (ac == 2)
	{
		try
		{
			Signal::setup();
			WebServ server(av[1]);
			server.run();
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
			return 1;
		}
	}
	else
	{
		std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
		return 1;
	}
	std::cout << "Server: Shutting down..." << std::endl;
	return 0;
}
