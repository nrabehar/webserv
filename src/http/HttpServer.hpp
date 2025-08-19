#ifndef	HTTP_SERVER_HPP
#define	HTTP_SERVER_HPP
#include "../conf/Config.hpp"
#include "../network/NetworkManager.hpp"
#include "../network/ConnectionManager.hpp"
#include "RequestManager.hpp"

class	HttpServer
{

	private:
		const Config&	_config;
		NetworkManager	*_net_manager;
		ConnectionManager	*_con_manager;
		RequestManager	*_req_manager;


	public:

		HttpServer(const Config& config);
		~HttpServer();

		void	start();
		void	listen();

	private:


		void	readRequest(int fd);
		void	writeRequest(int fd);
		void	errorRequest(int fd);

		HttpServer();
    HttpServer(const HttpServer&);
    HttpServer& operator=(const HttpServer&);

};

#endif
