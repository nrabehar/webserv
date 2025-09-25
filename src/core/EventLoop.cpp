#include "webserv.hpp"

EventLoop *	EventLoop::_inst = NULL;

EventLoop::~EventLoop() {}

EventLoop::EventLoop(): _running(false) {}


EventLoop &	EventLoop::instance()
{

	if (!_inst)
		_inst = new EventLoop();

	return (*_inst);

}

void	EventLoop::destroy()
{

	if (_inst)
		delete _inst;

	_inst = NULL;

}


void EventLoop::addHandler(IEventHandler * h, short events)
{ _poller.add(h, events); }

void EventLoop::modHandler(IEventHandler * h, short events)
{ _poller.mod(h, events); }

void EventLoop::delHandler(IEventHandler * h)
{ _poller.del(h); }

void EventLoop::run(int poll_timeout)
{

	_running = true;
	while (_running) {

		std::vector<std::pair<IEventHandler *, short> > events;
		events = _poller.pollOnce(poll_timeout);

		for (size_t i = 0; i < events.size(); ++i)
		{
		
			IEventHandler *h = events[i].first;
			short revents = events[i].second;

			h->handle(revents);
		
		}

	}

}

void EventLoop::stop() { _running = false; }