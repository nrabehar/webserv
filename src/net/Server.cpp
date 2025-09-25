#include "webserv.hpp"

Net::Server::Server(const ServerConfig::Listen & listen, const ServerConfig & conf)
	: EventHandler(-1), _conf(conf), _listen(listen), _infos(NULL)
{

	setup();

}
Net::Server::~Server() {}

void	Net::Server::handle(short e)
{

	switch (e)
	{
		case POLLIN:
			if (!acceptConnection())
				throw std::runtime_error("Could not accept client");
			break;
		case POLLOUT:
			break;		
		default:
			throw std::runtime_error("Server jsut down");
			break;
	}

}

void	Net::Server::setup()
{

	int	s = getAddrInfo();
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

}


int	Net::Server::getAddrInfo()
{

	const char * host = _listen.host.c_str();
	const char * port = _listen.port.c_str();

	std::cout << "Try to getaddrinfo: " << host << "===" << port << std::endl;
	return (::getaddrinfo(host, port, NULL, &_infos));

}

bool	Net::Server::acceptConnection()
{

	struct sockaddr_in	c_addr;
	socklen_t	c_len = sizeof(c_addr);

	int c_fd = accept(_fd, (struct sockaddr *)&c_addr, &c_len);
	if (c_fd == -1)
		return (false);

	Connection * client = new Connection(c_fd, this);
	EventLoop::instance().addHandler(client, POLLIN | POLLOUT);

	return (true);

}
