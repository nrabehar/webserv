#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP 1

#include "Logger.hpp"
#include <sstream>
#include <fstream>
#include <vector>

class ConfigFile
{
  private:
    std::string _name;
    std::string _content;
    std::vector<std::string> _server_blocks;
  public:
    ConfigFile(const std::string &);
    ~ConfigFile();
    const std::string & getName() const;
    const std::string & getContent() const;
    const std::vector<std::string> & getServerBlocks() const;
  private:
    ConfigFile();
    ConfigFile(const ConfigFile &);
    ConfigFile &operator=(const ConfigFile &);
    void createServerBlocks();
};

#endif // CONFIGFILE_HPP
