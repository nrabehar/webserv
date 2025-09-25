#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include "../webserv.hpp"


struct LocationConfig;

struct ServerConfig
{
	struct Listen
	{
		std::string port;
		std::string host;

		Listen();

	};

	size_t								keepalive_timeout;
	std::vector<Listen> 	listen;
	std::vector<LocationConfig> location;

	ServerConfig();

	void print() const;

};

struct LocationConfig
{
	std::string 																			path;
	std::string 																			root;
	bool																							autoindex;
	std::vector<std::string> 													index;
	std::map<int, std::string>												err_page;
	std::vector<std::pair<std::string, std::string> > cgi;

	LocationConfig();

};

#endif // CONFIG_SERVER_HPP