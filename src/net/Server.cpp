#include "webserv.hpp"

using namespace Net;

Server::Server(const ServerConfig::Listen & listen, const ServerConfig & conf)
  : _fd(-1), _conf(conf), _listen(listen), _infos(NULL)
{
  setup();
}

Server::~Server()
{
  if (_fd > -1)
    ::close(_fd);
  _fd = -1;
}

int Server::fd() const { return (_fd); }

void  Server::handle(short e)
{

  if (e & POLLIN)
  {
    if (!acceptConnection())
      throw std::runtime_error("Could not accept client");
  }
  if (e & POLLHUP)
    EventLoop::instance().delHandler(this);
  if (e & (POLLERR | POLLNVAL))
    throw std::runtime_error("Socket " + String::str(_fd) + " error");

}

void  Server::onTimeout() {}

void  Server::setup()
{

  int s = getAddrInfo();
  if (s != 0)
    throw std::runtime_error(std::string(gai_strerror(s)));

  _fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_fd == -1)
    throw std::runtime_error("Could not create socket");

  int opt_val = 1;
  if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) < 0)
    throw std::runtime_error("Cannot set SO_REUSEADDR");

  if (bind(_fd, _infos->ai_addr, _infos->ai_addrlen) == -1)
  {
    freeaddrinfo(_infos);
    throw std::runtime_error("Could not bind " + _listen.host + ":" + _listen.port + " " + std::string(strerror(errno)));
  }

  freeaddrinfo(_infos);

  if (listen(_fd, LISTEN_BACKLOG) == -1)
    throw std::runtime_error("Could not listen " + _listen.host + ":" + _listen.port);

  LOG("Listening on http://" + _listen.host + ":" + _listen.port);

}

int Server::getAddrInfo()
{

  const char * host = _listen.host.c_str();
  const char * port = _listen.port.c_str();

  return (::getaddrinfo(host, port, NULL, &_infos));

}

bool  Server::acceptConnection()
{

  struct sockaddr_in  c_addr;
  socklen_t c_len = sizeof(c_addr);

  int c_fd = accept(_fd, (struct sockaddr *)&c_addr, &c_len);
  if (c_fd == -1)
    return (false);

  Client * client = new Client(c_fd, this);
  EventLoop::instance().addHandler(client, POLLIN | POLLOUT);

  return (true);

}

const ServerConfig & Server::getConfig() const { return (_conf); }
