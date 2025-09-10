#include "webserv.hpp"

Config::~Config() {
}

Config::Config(const std::string & name) : _file(name) {
}

EValidator Config::validate() const {
	return (VALIDATOR_OK);
}

bool Config::parse(const std::string &) {
	return (true);
}

ConfigFile & Config::getFile() {
	return (_file);
}
