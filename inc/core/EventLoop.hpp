#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include "Poller.hpp"

class Config;

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
			short	event;
			IEventHandler *handler;

		};

		std::vector<PendingOp>	_pending;
		Config *_conf;

	public:

		~EventLoop();

		static EventLoop& instance();
		static void destroy();

		void addHandler(IEventHandler * h, short events);
		void modHandler(IEventHandler * h, short events);
		void delHandler(IEventHandler * h);

		void run(int poll_timeout = -1);
		void stop();

		void setConfig(Config * conf);

	private:

		EventLoop();
		EventLoop(const EventLoop &);
		EventLoop & operator=(const EventLoop &);

		void	applyPending();

};


#endif