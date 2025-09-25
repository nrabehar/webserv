#ifndef SERVER_HPP
#define SERVER_HPP

#include "Net.hpp"
#include "../config/Config.hpp"
#include "../core/EventHandler.hpp"
#include "../core/Poller.hpp"

#define	LISTEN_BACKLOG	128

class Net::Server: public EventHandler
{

	private:

		const ServerConfig & _conf;
		const ServerConfig::Listen & _listen;

		struct 	addrinfo	*_infos;

	public:

		Server(const ServerConfig::Listen & listen, const ServerConfig & conf);
		virtual ~Server();

		void handle(short e);

	private:

		Server();
		Server(const Server &);
		Server & operator=(const Server &);

		void	setup();
		int		getAddrInfo();
		bool	acceptConnection();

};

#endif // SERVER_HPP