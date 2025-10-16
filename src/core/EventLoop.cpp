#include "webserv.hpp"

EventLoop *	EventLoop::_inst = NULL;

EventLoop::~EventLoop()
{
	std::vector<PendingOp>::iterator it;
	for (it = _pending.begin(); it != _pending.end(); ++it)
	{
		if (it->type == OP_ADD)
			delete it->handler;
	}
	_pending.clear();
	if (_conf)
		delete _conf;
}

EventLoop::EventLoop(): _running(false), _conf(NULL) {}


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
{

	PendingOp pending;
	pending.type = OP_ADD;
	pending.event = events;
	pending.handler = h;

	_pending.push_back(pending);

}

void EventLoop::modHandler(IEventHandler * h, short events)
{

	PendingOp pending;
	pending.type = OP_MOD;
	pending.event = events;
	pending.handler = h;

	_pending.push_back(pending);

}

void EventLoop::delHandler(IEventHandler * h)
{

	PendingOp pending;
	pending.type = OP_DEL;
	pending.event = 0;
	pending.handler = h;

	_pending.push_back(pending);

}

void EventLoop::run(int poll_timeout)
{

	_running = true;
	while (_running) {

		applyPending();
		std::vector<std::pair<IEventHandler *, short> > events;
		events = _poller.pollOnce(poll_timeout);

		for (size_t i = 0; i < events.size(); ++i)
		{

			IEventHandler *h = events[i].first;
			short revents = events[i].second;

			h->handle(revents);

		}
		_poller.tick();

	}

}

void EventLoop::stop() { _running = false; }

void	EventLoop::applyPending()
{

	for (size_t i = 0; i < _pending.size(); ++i)
	{
		switch (_pending[i].type)
		{
			case OP_ADD:
				_poller.add(_pending[i].handler, _pending[i].event);
				break;
			case OP_MOD:
				_poller.mod(_pending[i].handler, _pending[i].event);
				break;
			case OP_DEL:
				_poller.del(_pending[i].handler);
				break;
			default: break;
		}
	}

	_pending.clear();

}

void EventLoop::setConfig(Config * conf)
{
	if (_conf)
		delete _conf;
	_conf = conf;
}
