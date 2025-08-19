#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Server.hpp"
#include "../tools/WException.hpp"
#include "../tools/String.hpp"
#include <fstream>
#include <map>
#include <vector>

class Config
{
private:
	Config();
	std::string config_path;
	std::string config_contents;
	std::vector<Server> servers;
	void loadFile();
	void parseConfigFile();
	void validateServer(const Server &);
	void validateLocation(const Location &);
	void validateHostPort();
	Server parseServer(std::istringstream &);
	Location parseLocation(std::istringstream &);
	void printServerConfig(const Server &) const;
	void printLocationConfig(const Location &) const;

public:
	~Config();
	Config(const Config &);
	Config(const std::string &);
	Config &operator=(const Config &);
	void validate();
	void printConfig() const;
	const std::vector<Server> &getServers() const;
};

#endif