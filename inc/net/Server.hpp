#ifndef SERVER_HPP
#define SERVER_HPP

#include "../webserv.hpp"
#include "Client.hpp"
#include <netdb.h>

#define	LISTEN_BACKLOG	128

namespace Net
{


class Server: public EventHandler
{

	private:

		const ServerConfig & _conf;
		const ServerConfig::Listen & _listen;

		struct 	addrinfo	*_infos;

	public:

		Server(const ServerConfig::Listen & listen, const ServerConfig & conf);
		virtual ~Server();

		void handle(short e);
		void onTimeout();
		const ServerConfig & getConfig() const;

	private:

		Server();
		Server(const Server &);
		Server & operator=(const Server &);

		void	setup();
		int		getAddrInfo();
		bool	acceptConnection();

};
	
}

#endif // SERVER_HPP