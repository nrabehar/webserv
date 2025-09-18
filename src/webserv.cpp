#include "webserv.hpp"

void test(IOriginator<Config> & cf)
{
	LOG("Initial config content:");
	LOG(cf.getState().getContent());
	LOG("Modifying config content...");
	cf.getState().setContent("Modified content");
	LOG("Current config content:");
	LOG(cf.getState().getContent());
}

static void _run(const char * configfile)
{
	(void)configfile;
	Signal().setup();
	ConfigFile cfgfile;
	cfgfile.setName(configfile);
	LOG(cfgfile.getName());
	test(cfgfile);
	LOG("Restoring config to initial state...");
	cfgfile.restore(cfgfile.save());
	LOG(cfgfile.getState().getContent());
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
