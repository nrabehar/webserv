#include "webserv.hpp"

ConfigFile::~ConfigFile() {
}

ConfigFile::ConfigFile(const std::string & name) : AFile(name) {
	ECheck tmp;
	tmp = validate();
	if (tmp != CHK_OK)
		reportError(tmp);
	if (openFile() == false) {
		throw std::runtime_error("Failed to open file: " + _name);
	}
	if (load() == false) {
		throw std::runtime_error("Failed to load file: " + _name);
	}
}

void ConfigFile::reportError(ECheck err) {
	switch (err) {
		case CHK_FILE_NOT_FOUND:
			throw std::runtime_error ("Configuration: File not found: " + _name);
			break;
		case CHK_FILE_NO_READ_PERMISSION:
			throw std::runtime_error ("Configuration: No read permission for: " + _name);
			break;
		case CHK_FILE_NAME_EMPTY:
			throw std::runtime_error ("Configuration: File name is empty.");
			break;
		default:
			break;
	}
}
