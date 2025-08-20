#include "ConfigFile.hpp"

ConfigFile::~ConfigFile()
{
  DBG(METHOD_NAME + " called.");
  DBG(METHOD_NAME + " completed.");
}

ConfigFile::ConfigFile(const std::string & filename) : _filename(filename), _content("")
{
  DBG(METHOD_NAME + " called.");
  if (_filename.empty())
  {
    throw std::invalid_argument("Filename cannot be empty.");
  }
  std::ifstream file(_filename.c_str());
  if (!file.is_open())
  {
    throw std::runtime_error("Could not open file: " + _filename);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  _content = buffer.str();
  file.close();
  LOG("Config file loaded from: " + _filename);
  if (_content.empty())
  {
    throw std::runtime_error("Config file is empty: " + _filename);
  }
  LOG("Config file content loaded successfully.");
  DBG(METHOD_NAME + " completed.");
}

const std::string & ConfigFile::getFilename() const
{
  DBG(METHOD_NAME + " called.");
  DBG(METHOD_NAME + " completed.");
  return (_filename);
}

const std::string & ConfigFile::getContent() const
{
  DBG(METHOD_NAME + " called.");
  DBG(METHOD_NAME + " completed.");
  return (_content);
}
