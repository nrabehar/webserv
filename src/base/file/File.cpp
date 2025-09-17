#include "webserv.hpp"

File::File(const std::string & path)
 : _fd(-1), _path(path) {}

File::~File() {}

const std::string & File::getPath() const {
	return (this->_path);
}

FileFactory::~FileFactory() {}
IFile * FileFactory::create(const std::string & path) {
	if (path.find("inmemory://") == 0)
		return (new InMemoryFile(path));
	return (new LocalFile(path));
}