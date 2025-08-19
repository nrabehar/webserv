#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include "Location.hpp"
#include <string>
#include <map>

class Server
{
private:
	std::string _root;
	std::vector<size_t> _ports;
	std::vector<std::string> _indexs;
	std::vector<std::string> _hosts;
	bool _isDefault;
	bool _autoIndex;
	size_t _MaxBodySise;
	std::map<int, std::string> _errorPages;
	std::vector<Location> _locations;

public:
	Server();
	~Server();
	Server(const Server &);
	Server &operator=(const Server &);

	bool getIsDefault() const;
	size_t getClientMaxBodySize() const;
	const std::string &getRoot() const;
	const std::vector<size_t> &getPorts() const;
	const std::vector<Location> &getLocations() const;
	const std::vector<std::string> &getHosts() const;
	const std::map<int, std::string> &getErrorPages() const;

	void addPort(size_t port);
	void setRoot(const std::string &root);
	void addHost(const std::string &host);
	void setIsDefault(bool isDefault);
	void setMaxBodySize(size_t size);
	void addLocation(const Location &location);
	void setErrorPage(int code, const std::string &page);
	void setProperty(const std::string &key, const std::string &value);
	void fixLocationData();

private:
	void parseHostPort(const std::string &hostPort);
};

#endif