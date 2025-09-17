#include "webserv.hpp"

LocalFile::LocalFile(const std::string & path)
 : File(path) {}

LocalFile::~LocalFile() {}

bool LocalFile::open() {
	_fd = ::open(this->_path.c_str(), O_RDONLY, 0644);
	// Get an instance of a PollManager and register the file descriptor
	return (this->_fd != -1);
}

void LocalFile::close() {
	if (this->_fd != -1) {
		// Get an instance of a PollManager and unregister the file descriptor
		::close(this->_fd);
		this->_fd = -1;
	}
}

size_t LocalFile::read(char * buf, size_t count) {
	if (this->_fd == -1) return 0;
	return (::read(this->_fd, buf, count));
}

size_t LocalFile::write(const char * buf, size_t count) {
	if (this->_fd == -1) return 0;
	return (::write(this->_fd, buf, count));
}

size_t LocalFile::size() const {
	if (this->_fd == -1) return 0;
	struct stat st;
	if (::fstat(this->_fd, &st) == -1) return 0;
	return (st.st_size);
}
