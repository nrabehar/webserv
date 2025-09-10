#include "webserv.hpp"

Config::~Config() {
}

Config::Config(const std::string & name) : _file(name), _parser(_file) {
}

ConfigFile & Config::getFile() {
	return (_file);
}

ConfigParser & Config::getParser() {
	return (_parser);
}