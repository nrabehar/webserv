#include "webserv.hpp"
#include <poll.h>

static void _run(const char * configfile)
{
	Signal().setup();
	Config cm(configfile);
	cm.load();
	const std::vector<ServerConfig>	& servers = cm.servers();

	for (size_t i = 0; i < servers.size(); ++i)
	{
		const std::vector<ServerConfig::Listen> & listens = servers[i].listen;
		for (size_t i = 0; i < listens.size(); ++i)
		{
			Net::Server * server = new Net::Server(listens[i], servers[i]);
			EventLoop::instance().addHandler(server, POLLIN);
		}
	}

	EventLoop::instance().run();
}

int main(int ac, char **av)
{

	std::string config_file;
	if (ac == 1)
		config_file = "conf/default.conf";
	else
		config_file = av[1];
	
	LOG("Starting web server...");
	try { _run(config_file.c_str()); }
	catch (std::exception & e)
	{

		ERR(e.what());
		EventLoop::destroy();
		LOG("Shutting down web server...");
		return (1);

	}
	EventLoop::destroy();
	LOG("Shutting down web server...");
	return (Signal::existcode);

}
