#include "webserv.hpp"

ConfigFile::~ConfigFile() {
}

ConfigFile::ConfigFile(const std::string & name) : AFile(name) {
	ECheck tmp;
	tmp = check();
	if (tmp != ST_OK)
		reportError(tmp);
	if (openFile() == false) {
		throw std::runtime_error("Failed to open file: " + _name);
	}
	if (load() == false) {
		throw std::runtime_error("Failed to load file: " + _name);
	}
}

void ConfigFile::reportError(EFileStatus err) {
	switch (err) {
		case ST_NOT_FOUND:
			throw std::runtime_error ("Configuration: File not found: " + _name);
			break;
		case ST_NO_READ_PERMISSION:
			throw std::runtime_error ("Configuration: No read permission for: " + _name);
			break;
		case ST_EMPTY_NAME:
			throw std::runtime_error ("Configuration: File name is empty.");
			break;
		default:
			break;
	}
}
