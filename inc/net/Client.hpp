#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../config/Config.hpp"
#include "../core/EventHandler.hpp"
#include "../core/Poller.hpp"
#include "../http/Parser.hpp"
#include "../http/Request.hpp"

namespace Net
{

	class Server;

	class Client: public EventHandler
	{

		private:

			Server *	_server;

			Buffer   _in;
			Buffer   _out;

			Http::Parser	_parser;
			Http::Request	_req;

			bool 		_keep_alive;
			time_t	_last_active;

		public:

			Client(int _fd, Server * server);
			~Client();

			void	handle(short e); 

		private:

			Client(const Client &);
			Client & operator=(const Client &);

			void	onRead();
			void	onWrite();
			void	onError();

			bool	readSocket();

	};
	
}

#endif // CLIENT_HPP