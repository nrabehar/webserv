#include "webserv.hpp"

Poller::Poller() {}
Poller::~Poller()
{

	_pfds.clear();
	_handlers.clear();

}

void Poller::add(IEventHandler * h, short events)
{

	struct pollfd p;

	p.fd = h->fd();
	p.events = events;
	p.revents = 0;

	_pfds.push_back(p);

}

void Poller::mod(IEventHandler * h, short events)
{

	int	fd = h->fd();

	for (size_t i = 0; i < _pfds.size(); ++i)
	{

		if (_pfds[i].fd != fd)
			continue;
		_pfds[i].events = events;
		return;

	}

	add(h, events);

}

void Poller::del(IEventHandler * h)
{
	int fd = h->fd();

	std::vector<struct pollfd> pfds;
	for (size_t i = 0; i < _pfds.size(); ++i)
	{

		if (_pfds[i].fd != fd)
			pfds.push_back(_pfds[i]);

	}

	_pfds.swap(pfds);
	_handlers.erase(fd);

}

std::vector<std::pair<IEventHandler *, short> > Poller::pollOnce(int timeout)
{

	std::vector<std::pair<IEventHandler *, short> > ready;
	if (_pfds.empty())
		return (ready);

	int ret = ::poll(&_pfds[0], _pfds.size(), timeout);
	if (ret <= 0)
		return (ready);

	for (size_t i = 0; i < _pfds.size(); ++i) {
		
		if (_pfds[i].revents != 0)
		{

			int fd = _pfds[i].fd;
			std::map<int, IEventHandler *>::iterator it = _handlers.find(fd);
			if (it != _handlers.end())
				ready.push_back(std::make_pair(it->second, _pfds[i].revents));
			_pfds[i].revents = 0;
		
		}

	}

	return (ready);

}
