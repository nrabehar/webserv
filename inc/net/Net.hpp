#ifndef NET_HPP
#define NET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Server.hpp"

class Net
{

	public:

		~Net();

		class Server;
		class Connection;

	private:


		Net();
		Net(const Net &);
		Net & operator=(const Net &);

};


#endif // NET_HPP