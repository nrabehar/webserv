#include "webserv.hpp"

Net::Connection::Connection(int fd, Net::Server * server)
	: EventHandler(fd), _server(server)
{

	std::cout << "Client connected: " << fd << std::endl;

}

Net::Connection::~Connection() {}

void	Net::Connection::handle(short e)
{

	(void) e;

}