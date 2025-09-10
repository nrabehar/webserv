#include "webserv.hpp"

Config::~Config() {
}

Config::Config(const std::string & name) : _file(name) {
}

ConfigFile & Config::getFile() {
	return (_file);
}
