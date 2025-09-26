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
	//TODO: create Handlers
	/**//* the begining of webserv project
	 * - POLLIN |	Requests 	- Methods
	 * 											-	Cgi
	 * - POLLOUT |	Response - Methods
	 * 											 - Cgi
	 * - POLLHUP |	Clean close
	 * 
	 * - errors
	 */
	

}