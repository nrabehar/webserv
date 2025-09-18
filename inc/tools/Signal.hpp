#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include "../webserv.hpp"

class Signal:
public virtual ISignal
{

	public:

		static bool terminate;
		static int existcode;

		Signal();
		virtual ~Signal();
		Signal(const Signal &);
		Signal & operator=(const Signal &);

		virtual void setup();

		static void handle(int);

};

#endif
