/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:43:47 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 17:39:32 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Config.hpp"

#include "Socket.hpp"

#include "Signal.hpp"

#include "EventManager.hpp"

class WebServ
{
private:
	Config _conf;
	bool _running;
	std::vector<Socket> _sockets;
	EventManager _event_manager;

public:
	~WebServ();
	WebServ(const std::string &);

	void run();
	void createSocket(const Server &);

private:
	WebServ();
	WebServ(const WebServ &);
	WebServ &operator=(const WebServ &);
};

#endif
