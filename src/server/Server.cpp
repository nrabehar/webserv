#include "webserv.hpp"

Server::Server():
	_autoindex(false),
	_root(""),
	_max_body_size(0),
	_location(),
	_hostport(),
	_index(),
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
		_root = src._root;
		_autoindex = src._autoindex;
		_max_body_size = src._max_body_size;
		_location = src._location;
		_hostport = src._hostport;
		_index = src._index;
		_errorpage = src._errorpage;
	}
	return (*this);
}

void Server::parse(const std::string &block)
{
	std::istringstream stream(block);
	std::string token;
	while (stream >> token)
	{
		if (token == "location") {
			Block::skipLocation(stream);
			continue;
		} else {
			std::string line;
			if (std::getline(stream, line)) {
				line = String::trim(line, " \t;");
				if (line.empty())
				 continue;
				token = String::toLower(token);
				setDirective(token, line);
			}
			else {
				throw std::runtime_error("ConfigParser: unexpected end of line after '" + token + "'");
			}
		}
	}
	std::vector<std::string> loc_blocks = Block::extractLocation(block);
	for (size_t i = 0; i < loc_blocks.size(); i++)
	{
		Location loc;
		loc.parse(loc_blocks[i]);
		_location.push_back(loc);
	}
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
const std::vector<AddrPort> & Server::getHostPort() const { return (_hostport); }
const std::map<EStatusCode, std::string> & Server::getErrorPage() const { return (_errorpage); }
const std::map<EStatusCode, std::string> & Server::getRedirect() const { return (_redirect); }
const std::string & Server::getRoot() const { return (_root); }
const std::vector<std::string> & Server::getIndex() const { return (_index); }

void Server::setDirective(const std::string &directive, const std::string &value)
{
	if (directive == "autoindex")
	_autoindex = (String::toLower(value) == "on");
	else if (directive == "root")
		_root = value;
	else if (directive == "index")
		_index = String::split(value, " \t");
	else if (directive == "max_body_size" || directive == "client_max_body_size")
		_max_body_size = String::toBytes(value);
	else if (directive == "listen")
		_hostport.push_back(ConfigParser::parseHostPort(value));
	else if (directive == "return")
	 	_redirect = ConfigParser::parseRedirect(value);
	else if (directive == "error_page")
	{
		std::map<EStatusCode, std::string> parsed = ConfigParser::parseErrorPage(value);
		std::map<EStatusCode, std::string>::const_iterator it;
		for (it = parsed.begin(); it != parsed.end(); ++it)
			_errorpage[it->first] = it->second;
	}
}
