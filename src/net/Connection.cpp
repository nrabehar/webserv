#include "webserv.hpp"

Net::Connection::Connection(int fd, Net::Server * server)
	: EventHandler(fd), _server(server) {}

Net::Connection::~Connection() {}

void	Net::Connection::handle(short e)
{

	(void) e;

}