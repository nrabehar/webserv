#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include "../webserv.hpp"

struct LocationConfig;

struct ServerConfig
{
  struct Listen
  {
    std::string port;
    std::string host;

    Listen();

  };

  size_t                keepalive_timeout;
  size_t                gateway_timeout;
  std::vector<Listen>   listen;
  std::vector<LocationConfig> location;

  ServerConfig();
  ServerConfig(const ServerConfig & other);
  ServerConfig & operator=(const ServerConfig & other);

  void print() const;

};

struct LocationConfig
{
  std::string                                       path;
  std::string                                       root;
  bool                                              autoindex;
  std::vector<std::string>                          index;
  std::map<int, std::string>                        err_page;
  std::vector<std::pair<std::string, std::string> > cgi;
  std::vector<std::string>                          methods;
  size_t                                            client_max_body_size;
  std::pair<int, std::string>                       redirect;
  std::string                                       upload_store;

  LocationConfig();
  LocationConfig(const LocationConfig & other);
  LocationConfig & operator=(const LocationConfig & other);

  bool allowsMethod(const std::string & method) const;

};

#endif
