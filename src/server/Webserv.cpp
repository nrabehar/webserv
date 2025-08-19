#include "Webserv.hpp"

WebServ::~WebServ()
{
	delete _http_server;
}

WebServ::WebServ(const std::string &configPath) : _conf(configPath)
{
	_conf.validate();
	_conf.printConfig();
	_http_server = new HttpServer(_conf);
}

void WebServ::run()
{
	_http_server->start();

	for (; !Signal::shouldStop();)
		_http_server->listen();
}
