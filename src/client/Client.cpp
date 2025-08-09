/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 01:22:05 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 02:41:05 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd) : _fd(fd) {}
Client::~Client()
{
	if (_fd >= 0)
	{
		close(_fd);
		std::cout << "Client " << _fd << ": closed" << std::endl;
	}
	_fd = -1;
}

int Client::getFd() const { return _fd; }
