#include "webserv.hpp"

InMemoryFile::InMemoryFile(const std::string &path) : File(path)
{
}

InMemoryFile::~InMemoryFile()
{
}

bool InMemoryFile::open()
{
	return true;
}

void InMemoryFile::close()
{
}

size_t InMemoryFile::read(char *buffer, size_t size)
{
	(void)buffer;
	return size;
}

size_t InMemoryFile::write(const char *buffer, size_t size)
{
	(void)buffer;
	return size;
}

size_t InMemoryFile::size() const
{
	return 0;
}
