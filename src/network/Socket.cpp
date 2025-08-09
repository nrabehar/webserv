/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:18:05 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 04:47:47 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(size_t port, const std::string &host, const Server &server)
		: _fd(-1), _port(port), _host(host), _server(server) {}

Socket::~Socket()
{
	if (_fd >= 0)
	{
		close(_fd);
		std::cout << "Close socket: " << _fd << std::endl;
	}
	_fd = -1;
}

int Socket::getFd() const { return _fd; }
const Server &Socket::getServer() const { return _server; }

void Socket::setup()
{
	struct addrinfo *addr;
	int opt = 1;

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
		throw WException("Unable to create socket");

	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw WException("Cannot set SO_REUSEADDR:  ");

	setNonBlock(_fd);

	if (getaddrinfo(_host.c_str(), toString(_port).c_str(), NULL, &addr) < 0)
		throw WException("Cannot get address info:  " + std::string(gai_strerror(errno)));

	if (::bind(_fd, addr->ai_addr, addr->ai_addrlen) < 0)
		throw WException("Unable to bind: " + _host + ":" + toString(_port));

	freeaddrinfo(addr);

	if (listen(_fd, 128) == -1)
		throw WException("Unable to listen: " + _host + ":" + toString(_port));

	std::cout << "Server listen on http://" + _host + ":" + toString(_port) << std::endl;
}

bool Socket::operator==(int fd) { return _fd == fd; }

void Socket::setNonBlock(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw WException("fcntl F_GETFL failed: ");

	if (fcntl(_fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw WException("fcntl F_SETFL failed: ");
}

int Socket::acceptConnection()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            std::cerr << "Accept failed: " << strerror(errno) << std::endl;
        return -1;
    }

		setNonBlock(client_fd);

    return client_fd;
}