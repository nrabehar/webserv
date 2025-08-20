#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP 1

#include "Logger.hpp"
#include <sstream>
#include <fstream>

class ConfigFile
{
  private:
    std::string _filename;
    std::string _content;
  public:
    ConfigFile(const std::string & filename);
    ~ConfigFile();
    const std::string & getFilename() const;
    const std::string & getContent() const;
  private:
    ConfigFile();
    ConfigFile(const ConfigFile & src);
    ConfigFile &operator=(const ConfigFile & src);
};

#endif // CONFIGFILE_HPP
