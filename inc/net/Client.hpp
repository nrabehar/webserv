#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../config/Config.hpp"
#include "../core/EventHandler.hpp"
#include "../core/Poller.hpp"
#include "../http/Parser.hpp"
#include "../http/Request.hpp"
#include "../handler/RequestHandler.hpp"

namespace Net
{

	class Server;

	class Client: public EventHandler
	{

		private:

			Server *	_server;
			Handler::RequestHandler _handler;

			Buffer   _in;

			Http::Parser	_parser;
			Http::Request	_req;
			Http::Response	_res;

			bool 		_keep_alive;
			char  *	_out;
			size_t	_out_size;

		public:

			Client(int _fd, Server * server);
			~Client();

			void	handle(short e);
			void	onTimeout();
			bool	keepAlive() const;
			void	setKeepAlive(bool keep_alive);
			Server *getServer() const;
			char *	out() const;
			void	setOut(char *out, size_t size);
			size_t	outSize() const;

		private:

			Client(const Client &);
			Client & operator=(const Client &);

			void	onRead();
			void	onWrite();
			void	onError();

			bool	readSocket();

	};

}

#endif
