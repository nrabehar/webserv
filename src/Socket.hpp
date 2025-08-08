/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:56:20 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 11:46:21 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "ServerConfig.hpp"
#include "unistd.h"

class Socket
{
private:
	int _fd;
	const ServerConfig &_server;

public:
	Socket(int fd, const ServerConfig &server);
	~Socket();

	int getFd() const;
	const ServerConfig &getServer() const;

	void setFd(int fd);

private:
	Socket();
};

#endif
