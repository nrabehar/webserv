/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 12:50:26 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 14:27:02 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::~Server() {}

Server::Server(const std::string &configPath) : _conf(configPath), _running(false)
{
	_conf.validate();
	_conf.printConfig();
}

Socket &Server::createSocket(const ServerConfig &server)
{
	const std::vector<size_t> ports = server.getPorts();
	const std::vector<std::string> hosts = server.getHosts();
	for (size_t i = 0; i < ports.size(); ++i)
		_sockets.push_back(Socket(ports[i], hosts[i], server));
	return (_sockets.back());
}

void Server::run()
{
	const std::vector<ServerConfig> &servers = _conf.getServers();
	std::vector<ServerConfig>::const_iterator it;
	for (it = servers.begin(); it != servers.end(); ++it)
		createSocket(*it);
	for (; _running && !Signal::shouldStop();)
	{
	}
	std::cout << "Server: Shutting down..." << std::endl;
}