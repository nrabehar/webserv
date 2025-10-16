#include "webserv.hpp"

bool Signal::terminate = false;

int Signal::existcode = 0;

std::map<int, std::string> signals;

Signal::Signal()
{
}

Signal::~Signal()
{
}

Signal::Signal(const Signal &)
{
}

Signal & Signal::operator=(const Signal &)
{
	return (*this);
}

void Signal::handle(int n)
{
	if (signals.find(n) != signals.end())
	{
		WRN("Received signal " + signals[n]);
		Signal::terminate = true; Signal::existcode = 128 + n;
		EventLoop::instance().stop();
	}
}

void Signal::setup()
{
	signals[SIGINT] = "SIGINT";
	signals[SIGTERM] = "SIGTERM";
	signals[SIGQUIT] = "SIGQUIT";

	if (std::signal(SIGINT, Signal::handle) == SIG_ERR)
	{ WRN("Running without signal SIGINT..."); }
	if (std::signal(SIGTERM, Signal::handle) == SIG_ERR)
	{ WRN("Running without signal SIGTERM..."); }
	if (std::signal(SIGQUIT, Signal::handle) == SIG_ERR)
	{ WRN("Running without signal SIGQUIT..."); }
}
