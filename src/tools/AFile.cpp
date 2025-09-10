#include "webserv.hpp"

AFile::~AFile() {
	closeFile();
}

AFile::AFile(const std::string & name) : _name(name), _stream(), _content() {
	if (validate() != VALIDATOR_OK) {
		throw std::runtime_error("Failed to validate file: " + _name);
	}
	if (openFile() == false) {
		throw std::runtime_error("Failed to open file: " + _name);
	}
	if (load() == false) {
		throw std::runtime_error("Failed to load file: " + _name);
	}
}

bool AFile::openFile() {
	if (_stream.is_open())
		return (true);
	_stream.open(_name.c_str());
	return (_stream.is_open());
}

void AFile::closeFile() {
	if (_stream.is_open()) {
		_stream.close();
	}
}

bool AFile::load() {
	if (!_stream.is_open())
		return (false);
	std::ostringstream oss;
	if (!(oss << _stream.rdbuf()))
		return (false);
	_content = oss.str();
	return (true);
}

EValidator AFile::validate() const {
	struct stat buf;
	if (stat(_name.c_str(), &buf) != 0)
		return (VALIDATOR_FILE_NOT_FOUND);
	if (access(_name.c_str(), R_OK) != 0)
		return (VALIDATOR_FILE_NO_READ_PERMISSION);
	if (_name.empty())
		return (VALIDATOR_FILE_NAME_EMPTY);
	return (VALIDATOR_OK);
}

const std::ifstream & AFile::getStream() const {
	return (_stream);
}

const std::string & AFile::getName() const {
	return (_name);
}

const std::string & AFile::getContent() const {
	return (_content);
}
