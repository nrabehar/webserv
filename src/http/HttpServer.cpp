#include "HttpServer.hpp"

HttpServer::HttpServer(const Config &config)
		: _config(config),
			_net_manager(NULL),
			_con_manager(NULL),
			_req_manager(NULL) {}

HttpServer::~HttpServer()
{
	delete _net_manager;
	delete _con_manager;
	delete _req_manager;
}

void HttpServer::start()
{
	std::cout << "HttpServer: Starting..." << std::endl;

	_net_manager = new NetworkManager();
	_con_manager = new ConnectionManager();
	_req_manager = new RequestManager();

	const std::vector<Server> &servers = _config.getServers();
	for (size_t i = 0; i < servers.size(); ++i)
		_net_manager->createSocket(servers[i]);

	std::cout << "HttpServer: Started successfully" << std::endl;
}

void HttpServer::listen()
{
	std::vector<EventInfo> events = _net_manager->getEvents();

	for (size_t i = 0; i < events.size(); ++i)
	{
		const EventInfo &event = events[i];

		if (event.events & POLLIN)
			readRequest(event.fd);

		if (event.events & POLLOUT)
			writeRequest(event.fd);

		if (event.events & (POLLERR | POLLHUP | POLLNVAL))
			errorRequest(event.fd);
	}

	_net_manager->cleanup();
	_con_manager->cleanup();
}

void HttpServer::readRequest(int fd)
{
	if (_net_manager->isServer(fd))
	{
		int c_fd = _net_manager->accept(fd);
		if (c_fd > 0)
		{
			_con_manager->connect(c_fd);
			_net_manager->addClient(c_fd);
		}
	}
	else
	{
		char buffer[1024];
		ssize_t bytes = _con_manager->readClientData(fd, buffer, sizeof(buffer));
		if (bytes > 0)
		{
			_req_manager->addReqData(fd, buffer, bytes);

			if (_req_manager->isReqComplete(fd))
				std::cout << "Buffer [" << bytes << "]:\n\n"
									<< buffer << std::endl;
		}
		else
		{
			_net_manager->disconnect(fd);
			_con_manager->disconnect(fd);
		}
	}
}

void HttpServer::writeRequest(int fd)
{
	_net_manager->disconnect(fd);
	_con_manager->disconnect(fd);
}

void HttpServer::errorRequest(int fd)
{
	std::cout << "Error on fd: " << fd << std::endl;
	if (!_net_manager->isServer(fd))
	{
		_net_manager->disconnect(fd);
		_con_manager->disconnect(fd);
	}
}
