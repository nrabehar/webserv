#include "webserv.hpp"

Net::Server::Server(const ServerConfig::Listen & listen, const ServerConfig & conf)
	: EventHandler(-1), _conf(conf), _listen(listen), _infos(NULL)
{

	setup();

}
Net::Server::~Server() {}

void	Net::Server::handle(short e)
{

	if (e == POLLIN)
	{

		if (!acceptConnection())
			throw std::runtime_error("Could not accept client");

	}

}

void	Net::Server::setup()
{

	int	s = getAddrInfo();
	if (s != 0)
		throw std::runtime_error("getaddrinfo: " + std::string(gai_strerror(s)));

	struct addrinfo *rp;
	for (rp = _infos; rp != NULL; rp = rp->ai_next)
	{

		_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (_fd == -1)
			continue ;

		int opt_val = 1;
		if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) < 0)
			throw std::runtime_error("Cannot set SO_REUSEADDR");

		if (bind(_fd, rp->ai_addr, rp->ai_addrlen) == 0)
			break ;

		std::cout << "try" << std::endl;

		::close(_fd);

	}

	freeaddrinfo(_infos);

	if (rp == NULL)
		throw std::runtime_error("Could not bind " + _listen.host + ":" + _listen.port + " " + std::string(strerror(errno)));

	if (listen(_fd, LISTEN_BACKLOG) == -1)
		throw std::runtime_error("Could not listen " + _listen.host + ":" + _listen.port);

}

int	Net::Server::getAddrInfo()
{

	struct addrinfo hints;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; //! maybe set family in  listen parser
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	const char * host = _listen.host.c_str();
	const char * port = _listen.port.c_str();

	return (::getaddrinfo(host, port, &hints, &_infos));

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
