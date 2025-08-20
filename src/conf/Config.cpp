#include "Config.hpp"

Config::~Config()
{
  DBG(METHOD_NAME + " called.");
}

Config::Config(const std::string & name)
  : _file(name)
{
  DBG(METHOD_NAME + " called with config file: " + name);
}
