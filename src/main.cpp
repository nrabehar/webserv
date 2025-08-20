#include "Webserv.hpp"

int main(int ac, char **av)
{
  std::string config_file_name = "webserv.conf";
  DEBUGMODE(true);
  DBG(METHOD_NAME + " called.");
  switch (ac)
  {
    case 1:
      break ;
    case 2:
      config_file_name = av[1];
      break ;
    default:
      ERR("Usage: " + TOSTRING(av[0]) + " [<config_file>]");
      return (1);
  }
  LOG("Starting application.");
  try
  {
    Webserv webserv(config_file_name);
  }
  catch (const std::exception & e)
  {
    ERR(e.what());
    return (1);
  }
  LOG("Application finished successfully.");
  return (0);
}
