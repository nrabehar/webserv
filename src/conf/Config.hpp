#ifndef CONFIG_HPP
#define CONFIG_HPP 1

#include "ConfigFile.hpp"

class Config
{
  private:
    ConfigFile _file;
  public:
    ~Config();
    Config(const std::string &);
  private:
    Config();
    Config(const Config &);
    Config & operator=(const Config &);
};

#endif // CONFIG_HPP
