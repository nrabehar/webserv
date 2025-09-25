#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Net.hpp"
#include "../config/Config.hpp"
#include "../core/EventHandler.hpp"
#include "../core/Poller.hpp"

class Net::Connection: public EventHandler
{

	private:

		Net::Server *	_server;

	public:

		Connection(int _fd, Net::Server * server);
		~Connection();

		void	handle(short e); 

	private:

		Connection(const Connection &);
		Connection & operator=(const Connection &);

};


#endif // CONNECTION_HPP