#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP 1

#include "Logger.hpp"
#include <sstream>
#include <fstream>

class ConfigFile
{
  private:
    std::string _name;
    std::string _content;
  public:
    ConfigFile(const std::string &);
    ~ConfigFile();
    const std::string & getName() const;
    const std::string & getContent() const;
  private:
    ConfigFile();
    ConfigFile(const ConfigFile &);
    ConfigFile &operator=(const ConfigFile &);
};

#endif // CONFIGFILE_HPP
