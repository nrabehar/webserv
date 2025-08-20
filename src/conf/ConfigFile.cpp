#include "ConfigFile.hpp"

ConfigFile::~ConfigFile()
{
  DBG(METHOD_NAME + " called.");
}

ConfigFile::ConfigFile(const std::string & name) : _name(name), _content("")
{
  DBG(METHOD_NAME + " called.");
  if (_name.empty())
  {
    throw std::invalid_argument("Filename cannot be empty.");
  }
  std::ifstream file(_name.c_str());
  if (!file.is_open())
  {
    throw std::runtime_error("Could not open file: " + _name);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  _content = buffer.str();
  file.close();
  LOG("Config file loaded from: " + _name);
  if (_content.empty())
  {
    throw std::runtime_error("Config file is empty: " + _name);
  }
  LOG("Config file content loaded successfully.");
}

const std::string & ConfigFile::getName() const
{
  DBG(METHOD_NAME + " called.");
  return (_name);
}

const std::string & ConfigFile::getContent() const
{
  DBG(METHOD_NAME + " called.");
  return (_content);
}
