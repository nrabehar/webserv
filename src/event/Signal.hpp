#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <csignal>

class Signal
{
private:
	static bool _stopReq;
	static void handle(int signum);

public:
	Signal();
	~Signal();

	static void setup();
	static bool shouldStop();
	static void reqStop();
};

#endif
