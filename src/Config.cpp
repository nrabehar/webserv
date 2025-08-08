/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:23:10 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 11:47:55 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "String.hpp"

Config::~Config() {}

Config::Config(const Config &other)
		: config_path(other.config_path),
			config_contents(other.config_contents),
			servers(other.servers) {}

Config::Config(const std::string &path) : config_path(path)
{
	loadFile();
	parseConfigFile();
}

Config &Config::operator=(const Config &other)
{
	if (this != &other)
	{
		this->servers = other.servers;
		this->config_path = other.config_path;
	}
	return *this;
}

void Config::loadFile()
{
	std::ifstream ifs(config_path.c_str());
	if (!ifs.is_open())
		throw WException("Cannot open config file: " + config_path);
	std::getline(ifs, config_contents, '\0');
	if (config_contents.empty())
		throw WException("Empty config file: " + config_path);
	ifs.close();
}

void Config::validate()
{
	if (servers.empty())
		throw ValidationException("No servers defined");
	validateHostPort();
	for (size_t i = 0; i < servers.size(); ++i)
		validateServer(servers[i]);
}

void Config::validateHostPort()
{
	std::map<std::string, size_t> _hp;
	for (size_t i = 0; i < servers.size(); ++i)
	{
		const ServerConfig &server = servers[i];
		const std::vector<std::string> &hosts = server.getHosts();
		const std::vector<size_t> &ports = server.getPorts();

		for (size_t j = 0; j < hosts.size(); ++j)
		{
			std::string hostPort = hosts[j] + ":" + toString(ports[j]);
			if (_hp.find(hostPort) != _hp.end())
			{
				throw ValidationException("Duplicate host:port combination: " + hostPort +
																	" (servers " + toString(_hp[hostPort] + 1) +
																	" and " + toString(i + 1) + ")");
			}

			_hp[hostPort] = i;
		}
	}
}

void Config::validateServer(const ServerConfig &server)
{
	const std::vector<std::size_t> &ports = server.getPorts();
	for (size_t i = 0; i < ports.size(); ++i)
		if (!isValidPort(ports[i]))
			throw ValidationException("Invalid port: " + toString(ports[i]));
	const std::vector<Location> &locations = server.getLocations();
	for (size_t i = 0; i < locations.size(); ++i)
		validateLocation(locations[i]);
}

void Config::validateLocation(const Location &location)
{
	if (location.getPath().empty())
		throw ValidationException("Location path cannot be empty");
	for (size_t i = 0; i < location.getMethods().size(); ++i)
		if (!isValidMethod(location.getMethods()[i]))
			throw ValidationException("Invalid HTTP method: " + location.getMethods()[i]);
}

const std::vector<ServerConfig> &Config::getServers() const
{
	if (servers.empty())
		throw WException("No servers configured");
	return servers;
}

void Config::printConfig() const
{
	std::cout << "=== Configuration Summary ===" << std::endl;
	std::cout << "Config file: " << config_path << std::endl;
	std::cout << "Number of servers: " << servers.size() << std::endl;
	std::cout << std::endl;

	for (size_t i = 0; i < servers.size(); ++i)
	{
		std::cout << "Server " << (i + 1) << ":" << std::endl;
		printServerConfig(servers[i]);
		std::cout << std::endl;
	}
}

void Config::printServerConfig(const ServerConfig &server) const
{
	std::cout << "  Root: " << server.getRoot() << std::endl;

	const std::vector<size_t> &ports = server.getPorts();
	std::cout << "  Ports: ";
	for (size_t i = 0; i < ports.size(); ++i)
	{
		if (i > 0) std::cout << ", ";
		std::cout << ports[i];
	}
	std::cout << std::endl;

	const std::vector<std::string> &hosts = server.getHosts();
	std::cout << "  Hosts: ";
	for (size_t i = 0; i < hosts.size(); ++i)
	{
		if (i > 0) std::cout << ", ";
		std::cout << hosts[i];
	}
	std::cout << std::endl;

	std::cout << "  Default server: " << (server.getIsDefault() ? "yes" : "no") << std::endl;
	std::cout << "  Max body size: " << server.getClientMaxBodySize() << " bytes" << std::endl;

	const std::map<int, std::string> &errorPages = server.getErrorPages();
	if (!errorPages.empty())
	{
		std::cout << "  Error pages:" << std::endl;
		for (std::map<int, std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it)
		{
			std::cout << "    " << it->first << " -> " << it->second << std::endl;
		}
	}

	const std::vector<Location> &locations = server.getLocations();
	if (!locations.empty())
	{
		std::cout << "  Locations:" << std::endl;
		for (size_t i = 0; i < locations.size(); ++i)
		{
			printLocationConfig(locations[i]);
		}
	}
}

void Config::printLocationConfig(const Location &location) const
{
	std::cout << "    Path: " << location.getPath() << std::endl;
	if (!location.getRoot().empty())
		std::cout << "      Root: " << location.getRoot() << std::endl;

	const std::vector<std::string> &methods = location.getMethods();
	if (!methods.empty())
	{
		std::cout << "      Methods: ";
		for (size_t i = 0; i < methods.size(); ++i)
		{
			if (i > 0) std::cout << ", ";
			std::cout << methods[i];
		}
		std::cout << std::endl;
	}

	const std::vector<std::string> &indexes = location.getIndexs();
	if (!indexes.empty())
	{
		std::cout << "      Index files: ";
		for (size_t i = 0; i < indexes.size(); ++i)
		{
			if (i > 0) std::cout << ", ";
			std::cout << indexes[i];
		}
		std::cout << std::endl;
	}

	std::cout << "      Autoindex: " << (location.getAutoIndex() ? "on" : "off") << std::endl;

	if (!location.getCgi().empty())
		std::cout << "      CGI: " << location.getCgi() << std::endl;

	if (!location.getUploadPath().empty())
		std::cout << "      Upload path: " << location.getUploadPath() << std::endl;

	if (location.getMaxBodySize() > 0)
		std::cout << "      Max body size: " << location.getMaxBodySize() << " bytes" << std::endl;

	if (location.hasRedirect())
	{
		const std::pair<int, std::string> &redirect = location.getRedirect();
		std::cout << "      Redirect: " << redirect.first << " -> " << redirect.second << std::endl;
	}
}
