/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:56:20 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 19:56:15 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Config.hpp"
#include "unistd.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <netdb.h>
#include <cstring>
#include <fcntl.h>

class Socket
{
private:
	int _fd;
	const size_t _port;
	const std::string _host;
	const Server &_server;

public:
	Socket(size_t port, const std::string &host, const Server &server);
	~Socket();

	int getFd() const;
	const Server &getServer() const;

	void setup();

	int acceptConnection();

	bool operator==(int fd);

private:
	void setNonBlock(int fd);
	Socket();
};

#endif
