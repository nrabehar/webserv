#include "webserv.hpp"
#include <poll.h>
#include "core/memory/memory.hpp"

static void _run(const char * configfile)
{
  Signal().setup();
  Config * conf = ft::alloc<Config>(configfile);
  EventLoop::instance().setConfig(conf);
  conf->load();
  const std::vector<ServerConfig> & servers = conf->servers();

  for (size_t i = 0; i < servers.size(); ++i)
  {
    const std::vector<ServerConfig::Listen> & listens = servers[i].listen;
    for (size_t j = 0; j < listens.size(); ++j)
    {
      Net::Server * server = new Net::Server(listens[j], servers[i]);
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
