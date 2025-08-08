/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:29:15 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 02:08:04 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "Socket.hpp"
#include <map>
#include <vector>
#include <poll.h>
#include <sys/poll.h>

class IEventHandler
{
public:
	virtual ~IEventHandler() {}
	virtual void onRead(int fd) = 0;
	virtual void onWrite(int fd) = 0;
	virtual void onError(int fd) = 0;
};

struct	EventInfo
{
	int	fd;
	int	revents;
};

class EventManager
{
private:
	std::vector<struct pollfd> _pfds;
	std::map<int, Socket *> _sockets;
	IEventHandler *_handler;
	static const int POLL_TIMEOUT = 1000;
	struct fdMatcher
	{
		int _target_fd;
		fdMatcher(int fd);
		bool operator()(const struct pollfd &pfd) const;
	};

public:
	EventManager();
	~EventManager();

	void addSocket(Socket *socket);
	void addClient(int fd);
	void removeHandled(int fd);
	bool hasReadEvent(int fd) const;
	bool hasWriteEvent(int fd) const;
	bool hasErrorEvent(int fd) const;

	void setHandler(IEventHandler *handler);

	std::vector<EventInfo> handleEvent();

	Socket *getSocket(int fd);

private:
	int waitEvents();
	EventManager(const EventManager &);
	EventManager &operator=(const EventManager &);
};

#endif
