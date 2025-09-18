#include "webserv.hpp"

/**
 * AFile
 */
File::File(const std::string & path)
	: _fd(-1), _complete(false), _data(""), _path(path) {}

File::~File()
{
	if (_fd != -1)
		::close(_fd);
}

int File::getFD() const { return _fd; }
bool File::isComplete() const { return _complete; }

void File::read() {}

std::string File::getPath() const { return _path; }
std::string File::getData() const { return _data; }


/**
 * LocalFile
 */

LocalFile::LocalFile(const std::string & path)
	: File(path)
{
	_fd = ::open(path.c_str(), O_RDONLY|O_NONBLOCK);
}
LocalFile::~LocalFile(){}

void LocalFile::read()
{
	if (_fd == -1 || _complete)
		return ;
	
	char buf[4096];
	ssize_t n = ::read(_fd, buf, sizeof(buf));
	if (n > 0)
		_data.append(buf, n);
	else if (n == 0)
		_complete = true;
	else
	{
		close(_fd);
		_fd = -1;
		throw std::runtime_error("Error reading file: " + _path + " (" + std::string(std::strerror(errno)) + ")");
	}
	if (n <= 6096 && n > 0)
		_complete = true;
}

/**
 * InMemoryFile
 */
InMemoryFile::InMemoryFile(const std::string &path, const std::string &data)
	: File(path) { _data = data;}
InMemoryFile::~InMemoryFile(){}

/**
 * FileProxy
 */

FileProxy::FileProxy(IFile * file)
	: _file(file), _path(file->getPath()) {}

FileProxy::~FileProxy() { delete _file; }

int FileProxy::getFD() const { return _file->getFD(); }
bool FileProxy::isComplete() const
{
	CacheManager::getInstance()->use(CAT_FILE);

	if (CacheManager::getInstance()->exists(_path))
		return true;
	return _file->isComplete();
}

void FileProxy::read()
{
	CacheManager::getInstance()->use(CAT_FILE);
	if (isComplete())
		return ;

	_file->read();

	if (_file->isComplete())
		CacheManager::getInstance()->put(_path, _file->getData());
}

std::string FileProxy::getPath() const { return _path; }

std::string FileProxy::getData() const
{
	CacheManager::getInstance()->use(CAT_FILE);
	if (CacheManager::getInstance()->exists(_path))
		return CacheManager::getInstance()->get(_path);
	return _file->getData();
}

/**
 * FileFactory
 */
FileFactory::~FileFactory() {}
IFile*	FileFactory::create(const std::string &path)
{
	CacheManager *cache = CacheManager::getInstance();

	cache->use(CAT_FILE);
	
	if (cache->exists(path))
	{
		std::cout << "File: " << path << " is in cache" << std::endl;
		return (new InMemoryFile(path, cache->get(path)));
	}

	IFile *file = new LocalFile(path);
	if (file->getFD() == -1)
	{
		delete file;
		return (NULL);
	}
	return (new FileProxy(file));
}
