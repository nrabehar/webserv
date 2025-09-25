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
{

	PendingOp op;
	op.type = OP_ADD;
	op.handler = h;
	op.event = events;

	_pending.push_back(op);

}

void EventLoop::modHandler(IEventHandler * h, short events)
{

	PendingOp op;
	op.type = OP_MOD;
	op.handler = h;
	op.event = events;

	_pending.push_back(op);

}

void EventLoop::delHandler(IEventHandler * h)
{

	PendingOp op;
	op.type = OP_DEL;
	op.handler = h;
	op.event = 0;

	_pending.push_back(op);

}

void EventLoop::applyPending()
{

	for (size_t i = 0; i < _pending.size(); ++i)
	{

		PendingOp &op = _pending[i];
		switch (op.type)
		{
			
			case OP_ADD:
				_poller.add(op.handler, op.event);
				break;
			case OP_MOD:
				_poller.mod(op.handler, op.event);
				break;
			case OP_DEL:
				_poller.del(op.handler);
				break;
			default:
				break;

		}

	}
	_pending.clear();

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

	}

}

void EventLoop::stop() { _running = false; }