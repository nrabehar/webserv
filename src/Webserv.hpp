#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "http/HttpServer.hpp"
#include "event/Signal.hpp"

class WebServ
{
	private:
		Config _conf;
		HttpServer	*_http_server;

	public:

		~WebServ();
		WebServ(const std::string &);

		void 	run();

	private:

		WebServ();
		WebServ(const WebServ &);
		WebServ &operator=(const WebServ &);
};

#endif
