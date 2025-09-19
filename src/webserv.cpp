#include "webserv.hpp"
#include <poll.h>

void test(IOriginator<Config> &)
{
}

static void _run(const char * configfile)
{
	Signal().setup();
	Config cm(configfile);
	cm.load();
	cm.parse();
}

int main(int ac, char **av)
{
	if (ac != 2) { ERR("Usage: ./webserv <configfile>"); return (1); }
	LOG("Starting web server...");
	try { _run(av[1]); }
	catch (std::exception & e) { ERR(e.what()); return (1); }
	LOG("Shutting down web server...");
	return (Signal::existcode);
}
