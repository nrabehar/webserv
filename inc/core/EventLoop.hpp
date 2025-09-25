#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include "Poller.hpp"

class EventLoop
{

	private:

		Poller	_poller;
		bool	_running;
		static EventLoop	*	_inst;

		enum OpType { OP_ADD, OP_MOD, OP_DEL };
		struct PendingOp
		{

			OpType	type;
			IEventHandler *	handler;
			short	event;

		};

		std::vector<PendingOp>	_pending;

	public:

		~EventLoop();

		static EventLoop& instance();
		static void destroy();

		void addHandler(IEventHandler * h, short events);
		void modHandler(IEventHandler * h, short events);
		void delHandler(IEventHandler * h);

		void run(int poll_timeout = -1);
		void stop();

	private:

		EventLoop();
		EventLoop(const EventLoop &);
		EventLoop & operator=(const EventLoop &);

		void	applyPending();

};


#endif // EVENT_LOOP_HPP