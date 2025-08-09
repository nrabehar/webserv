/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:29:15 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 04:35:56 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "../network/Socket.hpp"
#include <iostream>
#include <unistd.h>
#include <map>
#include <vector>
#include <poll.h>
#include <algorithm>

#define	POLL_TIMEOUT 1000

struct	EventInfo
{
	int	fd;
	int	events;
};

class EventManager
{
	private:
		std::vector<struct pollfd> _pfds;
		std::map<int, Socket *> _sockets;

	public:
		EventManager();
		~EventManager();

		void addSocket(Socket *socket);
		void addClient(int fd);
		void removeHandled(int fd);

		std::vector<EventInfo> handleEvent();

		Socket *getSocket(int fd);

	private:
		int waitEvents();
		EventManager(const EventManager &);
		EventManager &operator=(const EventManager &);
};

#endif
