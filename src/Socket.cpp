/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:18:05 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 15:16:12 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(size_t port, const std::string &host, const Server &server)
		: _fd(-1), _port(port), _host(host), _server(server)
{
	_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (_fd < 0)
		throw WException("Unable to create socket: " + std::string(strerror(errno)));
	this->setup();
}

Socket::~Socket()
{
	if (_fd >= 0)
		close(_fd);
	_fd = -1;
}

int Socket::getFd() const { return _fd; }
const Server &Socket::getServer() const { return _server; }

void Socket::setup()
{
	struct addrinfo *addr;
	int opt = 1;

	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw WException("Cannot set SO_REUSEADDR:  " + std::string(strerror(errno)));
	if (getaddrinfo(_host.c_str(), toString(_port).c_str(), NULL, &addr))
		throw WException("Cannot get address info:  " + std::string(gai_strerror(errno)));
	if (::bind(_fd, addr->ai_addr, addr->ai_addrlen) < 0)
		throw WException("Unable to bind: " + _host + ":" + toString(_port));
	freeaddrinfo(addr);
	if (listen(_fd, 128) < 0)
		throw WException("Unable to listen: " + _host + ":" + toString(_port));
	std::cout << "Server listen on http://" + _host + ":" + toString(_port) << std::endl;
}

bool Socket::operator==(int fd) { return _fd == fd; }