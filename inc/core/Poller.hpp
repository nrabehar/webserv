#ifndef POLLER_HPP
#define POLLER_HPP

#include "EventHandler.hpp"
#include <vector>
#include <map>
#include <poll.h>


class Poller
{

	private:

		std::vector<struct pollfd>	_pfds;
		std::map<int, IEventHandler * >	_handlers;

	public:

		Poller();
		~Poller();

		void add(IEventHandler * h, short events);
		void mod(IEventHandler * h, short events);
		void del(IEventHandler * h);

		std::vector<std::pair<IEventHandler *, short> > pollOnce(int timeout);

	private:

		Poller(const Poller &);
		Poller & operator=(const Poller &);

};


#endif // POLLER_HPP