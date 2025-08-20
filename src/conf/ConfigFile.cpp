#include "ConfigFile.hpp"

ConfigFile::~ConfigFile()
{
  DBG(METHOD_NAME + " called.");
}

void ConfigFile::createServerBlocks()
{
  DBG(METHOD_NAME + " called.");
  // This function should parse the content and fill _server_blocks.
  LOG("Server blocks created successfully.");
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
  createServerBlocks();
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

const std::vector<std::string> & ConfigFile::getServerBlocks() const
{
  DBG(METHOD_NAME + " called.");
  return (_server_blocks);
}
