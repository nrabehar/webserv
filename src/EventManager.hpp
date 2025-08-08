/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:29:15 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 15:01:45 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "Socket.hpp"
#include <map>
#include <poll.h>

// PollManager in tmp/...steps.md
class EventManager
{
private:
	std::vector<struct pollfd> _pfds;
	std::map<int, Socket *> _sockets;
	static const int POLL_TIMEOUT = 1000; // 1s

	struct fdMatcher
	{
		int _target_fd;
		fdMatcher(int fd);
		bool operator()(const struct pollfd &pfd) const;
	};

public:
	EventManager();
	~EventManager();

	int wait();
	void addSocket(Socket *socket);
	void removeSocket(int fd);
	bool hasReadEvent(int fd) const;
	bool hasWriteEvent(int fd) const;
	bool hasErrorEvent(int fd) const;

	Socket *getScoket(int fd);

private:
	EventManager(const EventManager &);
	EventManager &operator=(const EventManager &);
};

#endif
