#include "webserv.hpp"

ServerConfig::ServerConfig()
	: keepalive_timeout(30),
		gateway_timeout(30) {}

void	ServerConfig::print() const
{
	std::cout << "Server:" << std::endl;
	std::cout << "  Keepalive timeout: " << keepalive_timeout << std::endl;
	std::cout << "  Gateway timeout: " << gateway_timeout << std::endl;

	std::cout << "  Listens:" << std::endl;
	for (size_t i = 0; i < listen.size(); ++i)
		std::cout << "    Host: " << listen[i].host << ", Port: " << listen[i].port << std::endl;

	std::cout << "  Locations:" << std::endl;
	for (size_t i = 0; i < location.size(); ++i)
	{
		const LocationConfig &loc = location[i];
		std::cout << "    Path: " << loc.path << std::endl;
		std::cout << "      Root: " << loc.root << std::endl;
		std::cout << "      Autoindex: " << (loc.autoindex ? "on" : "off") << std::endl;

		std::cout << "      Index files: ";
		for (size_t j = 0; j < loc.index.size(); ++j)
			std::cout << loc.index[j] << " ";
		std::cout << std::endl;

		std::cout << "      Error pages:" << std::endl;
		for (std::map<int,std::string>::const_iterator it = loc.err_page.begin(); it != loc.err_page.end(); ++it)
			std::cout << "        " << it->first << " -> " << it->second << std::endl;

		std::cout << "      CGI:" << std::endl;
		for (size_t j = 0; j < loc.cgi.size(); ++j)
			std::cout << "        " << loc.cgi[j].first << " -> " << loc.cgi[j].second << std::endl;
	}
}

ServerConfig::Listen::Listen()
	: port("80"), host("0.0.0.0") {}

LocationConfig::LocationConfig()
	: path(""),
		root(""),
		autoindex(false),
		client_max_body_size(DEFAULT_MAX_BODY_SIZE),
		redirect(std::make_pair(0, "")) {}

bool	LocationConfig::allowsMethod(const std::string & method) const
{

	if (methods.empty())
		return (true);

	for (size_t i = 0; i < methods.size(); ++i)
	{
		if (methods[i] == method)
			return (true);
	}

	return (false);

}
