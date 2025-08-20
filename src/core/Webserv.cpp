#include "Webserv.hpp"

Webserv::~Webserv()
{
  DBG(METHOD_NAME + " called.");
}

Webserv::Webserv(const std::string & filename)
  : _config(filename)
{
  DBG(METHOD_NAME + " called with config file: " + filename);
}
