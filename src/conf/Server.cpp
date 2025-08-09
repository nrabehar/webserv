/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:20:42 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 14:45:02 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Server::Server()
		: _root("./www"),
			_isDefault(false),
			_autoIndex(false),
			_MaxBodySise(1048576) {}

Server::~Server() {}

Server::Server(const Server &src)
		: _root(src._root),
			_ports(src._ports),
			_indexs(src._indexs),
			_hosts(src._hosts),
			_isDefault(src._isDefault),
			_autoIndex(src._autoIndex),
			_MaxBodySise(src._MaxBodySise),
			_errorPages(src._errorPages),
			_locations(src._locations) {}

Server &Server::operator=(const Server &src)
{
	if (this == &src)
		return (*this);
	_ports = src._ports;
	_hosts = src._hosts;
	_isDefault = src._isDefault;
	_MaxBodySise = src._MaxBodySise;
	_errorPages = src._errorPages;
	_root = src._root;
	_locations.clear();
	for (size_t i = 0; i < src._locations.size(); ++i)
	{
		Location location(src._locations[i]);
		_locations.push_back(location);
	}
	return (*this);
}

const std::string &Server::getRoot() const { return _root; }
const std::vector<size_t> &Server::getPorts() const { return _ports; }
const std::vector<std::string> &Server::getHosts() const { return _hosts; }
bool Server::getIsDefault() const { return _isDefault; }
size_t Server::getClientMaxBodySize() const { return _MaxBodySise; }
const std::map<int, std::string> &Server::getErrorPages() const { return _errorPages; }
const std::vector<Location> &Server::getLocations() const { return _locations; }

void Server::addPort(size_t port) { _ports.push_back(port); }
void Server::addHost(const std::string &host) { _hosts.push_back(host); }
void Server::setIsDefault(bool isDefault) { _isDefault = isDefault; }
void Server::setMaxBodySize(size_t size) { _MaxBodySise = size; }
void Server::setErrorPage(int code, const std::string &page) { _errorPages[code] = page; }
void Server::addLocation(const Location &location) { _locations.push_back(location); }
void Server::setRoot(const std::string &newRoot) { this->_root = newRoot; }

void Server::fixLocationData()
{
	std::vector<Location>::iterator it;
	for (it = _locations.begin(); it != _locations.end(); ++it)
	{
		Location &location = *it;
		if (location.getRoot().empty())
			location.setRoot(_root);
		if (!location.getMaxBodySize())
			location.setMaxBodySize(_MaxBodySise);
	}
}

void Server::parseHostPort(const std::string &hostPort)
{
	int port;
	std::string host;

	size_t colonPos = hostPort.find_last_of(':');
	if (colonPos == std::string::npos)
		throw std::invalid_argument("Invalid host:port syntax");
	host = hostPort.substr(0, colonPos);
	std::string portStr = hostPort.substr(colonPos + 1);
	if (!isNumeric(portStr))
		throw std::invalid_argument("Invalid port: " + portStr);
	port = std::atoi(portStr.c_str());
	if (host != "localhost" && host != "0.0.0.0" && !isValidIP(host))
		throw std::invalid_argument("Invalid IP address: " + host);
	if (!isValidPort(port))
		throw std::invalid_argument("Invalid port: " + toString(port));
	_ports.push_back(port);
	_hosts.push_back(host);
}

void Server::setProperty(const std::string &key, const std::string &value)
{
	if (key == "listen")
	{
		std::vector<std::string> parts = splitStr(value, ' ');
		for (size_t i = 0; i < parts.size(); ++i)
			parseHostPort(parts[i]);
	}
	else if (key == "root")
		_root = value;
	else if (key == "client_max_body_size" || key == "max_body_size")
		_MaxBodySise = toBytes(value);
	else if (key == "error_page")
	{
		std::vector<std::string> parts = splitStr(value, ' ');
		if (parts.size() >= 2)
		{
			std::string error_page = parts[parts.size() - 1];
			for (size_t i = 0; i < parts.size() - 1; ++i)
			{
				int code = std::atoi(parts[i].c_str());
				_errorPages[code] = error_page;
			}
		}
		else
			throw ParseException("Invalid declaration: " + key + " : " + value);
	}
	else
		throw ParseException("Unknown server key: " + key);
}