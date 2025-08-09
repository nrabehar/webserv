/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:35:15 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 05:26:56 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

WebServ::~WebServ()
{
	delete _http_server;
}

WebServ::WebServ(const std::string &configPath) : _conf(configPath)
{
	_conf.validate();
	_conf.printConfig();
	_http_server = new HttpServer(_conf);
}

void WebServ::run()
{
	_http_server->start();

	for (; !Signal::shouldStop();)
		_http_server->listen();
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
