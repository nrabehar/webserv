#include "webserv.hpp"

File::File(const std::string & path)
	: _fd(-1), _complete(false), _data(""), _path(path) {}

File::~File()
{
	if (_fd >= 0)
		::close(_fd);
}

int File::fd() const { return _fd; }
bool File::isComplete() const { return _complete; }

const std::string & File::getPath() const { return _path; }
const std::string & File::getData() const { return _data; }
ssize_t	File::write(const char * data, size_t n)
{

	if (_fd < 0)
		return (0);

	ssize_t	bytes = ::write(_fd, data, n);
	if (bytes <= 0)
	{
		::close(_fd);
		_fd = -1;
		return (-1);
	}
	return (bytes);

}

LocalFile::LocalFile(const std::string & path, int oflag)
	: File(path)
{
	_fd = ::open(path.c_str(), oflag | O_NONBLOCK, 00644);
}
LocalFile::~LocalFile(){}

ssize_t LocalFile::read()
{
	if (_fd == -1 || _complete)
		return (-1);

	char buf[4096];
	ssize_t n = ::read(_fd, buf, sizeof(buf));
	if (n <= 0)
	{
		close(_fd);
		_fd = -1;
		return (-1);
	}
	_data.append(buf, n);
	if (n < 4096)
		_complete = true;
	return (n);

}

InMemoryFile::InMemoryFile(const std::string &path, const std::string &data)
	: File(path) { _data = data; _fd = IN_MEMORY_FD; _complete = true; }
InMemoryFile::~InMemoryFile(){}
ssize_t InMemoryFile::read()
{

	_complete = true;
	return (_data.size());

}

FileProxy::FileProxy(IFile * file)
	: _file(file), _path(file->getPath()) {}

FileProxy::~FileProxy() { delete _file; }

int FileProxy::fd() const { return _file->fd(); }
bool FileProxy::isComplete() const
{
	CacheManager::getInstance()->use(CAT_FILE);

	if (CacheManager::getInstance()->exists(_path))
		return true;
	return _file->isComplete();
}

ssize_t FileProxy::read()
{
	CacheManager::getInstance()->use(CAT_FILE);
	if (isComplete())
		return (_file->getData().size());

	ssize_t n = _file->read();

	if (_file->isComplete())
		CacheManager::getInstance()->put(_path, _file->getData());
	return (n);

}

ssize_t	FileProxy::write(const char * data, size_t n)
{

	return (_file->write(data, n));

}

const std::string & FileProxy::getPath() const { return _path; }

const std::string & FileProxy::getData() const
{
	return (_file->getData());
}

FileFactory::~FileFactory() {}
IFile*	FileFactory::create(const std::string &path, int oflag)
{
	IFile	* file = NULL;

	file = new LocalFile(path, oflag);

	if (!file)
		return (NULL);

	if (file->fd() == -1)
	{
		delete file;
		return (NULL);
	}
	return (file);
}
