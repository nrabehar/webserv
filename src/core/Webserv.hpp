#ifndef WEBSERV_HPP
#define WEBSERV_HPP 1

#include "Config.hpp"

class Webserv
{
  private:
    Config _config;
  public:
    ~Webserv();
    Webserv(const std::string &);
  private:
    Webserv();
    Webserv(const Webserv &);
    Webserv & operator=(const Webserv &);
};

#endif // WEBSERV_HPP
