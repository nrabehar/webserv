#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include "../webserv.hpp"

struct Config::Server
{
	struct Listen
	{
		std::string port;
		std::string host;

		Listen();

	};

	struct Location
	{
		std::string 																			path;
		std::string 																			root;
		bool																							autoindex;
		std::vector<std::string> 													index;
		std::map<int, std::string>												err_page;
		std::vector<std::pair<std::string, std::string> > cgi;

		Location();

	};

	size_t								keepalive_timeout;
	std::vector<Listen> 	listen;
	std::vector<Location> location;

	Server();

	void print() const;

};

#endif // CONFIG_SERVER_HPP