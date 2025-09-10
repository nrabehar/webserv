#include "webserv.hpp"

Server::Server():
	_autoindex(false),
	_max_body_size(0),
	_location(),
	_hostport(),
	_errorpage() {
}

Server::~Server() {
}

Server::Server(const Server & src) {
	(*this) = src;
}

Server & Server::operator=(const Server & src)
{
	if (this != &src) {
		_autoindex = src._autoindex;
		_max_body_size = src._max_body_size;
		_location = src._location;
		_hostport = src._hostport;
		_errorpage = src._errorpage;
	}
	return (*this);
}

void Server::parse(const std::string &)
{
	// @todo Implementation of the parse function
}

EErrorCode Server::check() const
{
	// @todo Implementation of the check function
	return (ST_OK);
}

void Server::reportError(EErrorCode code)
{
	switch (code)
	{
		default:
			break;
	}
}

bool Server::getAutoIndex() const { return (_autoindex); }
size_t Server::getMaxBodySize() const { return (_max_body_size); }
const std::vector<Location> & Server::getLocation() const { return (_location); }
const std::vector<ServerHostPort> & Server::getHostPort() const { return (_hostport); }
const std::map<EStatusCode, std::string> & Server::getErrorPage() const { return (_errorpage); }
