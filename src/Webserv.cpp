/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:35:15 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 15:13:07 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

WebServ::~WebServ() {}

WebServ::WebServ(const std::string &configPath) : _conf(configPath), _running(false)
{
	_conf.validate();
	_conf.printConfig();
}

Socket &WebServ::createSocket(const Server &server)
{
	const std::vector<size_t> ports = server.getPorts();
	const std::vector<std::string> hosts = server.getHosts();
	for (size_t i = 0; i < ports.size(); ++i)
		_sockets.push_back(Socket(ports[i], hosts[i], server));
	return (_sockets.back());
}

void WebServ::run()
{
	const std::vector<Server> &servers = _conf.getServers();
	std::vector<Server>::const_iterator it;
	EventManager event_manager;
	for (it = servers.begin(); it != servers.end(); ++it)
	{
		Socket &socket = createSocket(*it);
		event_manager.addSocket(&socket);
	}
	_running = true;
	for (; _running && !Signal::shouldStop();)
	{
		int revent = event_manager.wait();
		(void)revent;
	}
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
