/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:35:15 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 20:53:48 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

WebServ::~WebServ() {}

WebServ::WebServ(const std::string &configPath) : _conf(configPath), _running(false)
{
	_conf.validate();
	_conf.printConfig();
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

void WebServ::run()
{
	const std::vector<Server> &servers = _conf.getServers();
	std::vector<Server>::const_iterator it;
	for (it = servers.begin(); it != servers.end(); ++it)
		createSocket(*it);
	for (; !Signal::shouldStop();)
		_event_manager.handleEvent();
	std::cout << "Server: Shutting down..." << std::endl;
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
	return 0;
}
