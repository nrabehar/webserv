/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 12:45:05 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 14:26:10 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"
#include "Socket.hpp"
#include "Signal.hpp"

class Server
{
private:
	Config _conf;
	bool _running;
	std::vector<Socket> _sockets;

public:
	~Server();
	Server(const std::string &);

	void run();
	Socket &createSocket(const ServerConfig &);

private:
	Server();
	Server(const Server &);
	Server &operator=(const Server &);
};

#endif