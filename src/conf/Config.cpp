#include "webserv.hpp"

Config::~Config() {
}

Config::Config(const std::string & name) : _file(name), _parser(_file) {
	_parser.parse("");
}

ConfigFile & Config::getFile() {
	return (_file);
}

ConfigParser & Config::getParser() {
	return (_parser);
}

const std::vector<Server> & Config::getServer() const {
	return (_parser.getServer());
}
