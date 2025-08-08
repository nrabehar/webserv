/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:18:05 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 11:45:59 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int fd, const ServerConfig &server) : _fd(fd), _server(server) {}
Socket::~Socket()
{
	if (_fd >= 0)
		close(_fd);
	_fd = -1;
}

int Socket::getFd() const { return _fd; }
const ServerConfig &Socket::getServer() const { return _server; }

void Socket::setFd(int fd) { _fd = fd; }