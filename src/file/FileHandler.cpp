#include "webserv.hpp"

/**
 * FileHandler base class implementation
 */

FileHandler::FileHandler()
	: _next(NULL), _prev(NULL), _reason(""), _file(NULL) {}
FileHandler::~FileHandler() {
	_file = NULL;
}
IFileHandler* FileHandler::setNext(IFileHandler* next) {
	_next = next;
	_next->setPrev(this);
	return next;
}
IFileHandler* FileHandler::setPrev(IFileHandler* prev) {
	_prev = prev;
	return prev;
}
IFile* FileHandler::handle(const std::string& path) {
	reset_chain();
	if (_next)
	{
		_next->setFile(_file);
		_file = NULL;
	}
	if (_next && _reason.empty()) {
		return (_next->handle(path));
	}
	if (!_reason.empty())
	{
		if (_file)
			delete _file;
		_file = NULL;
	}
	return (_file);
}
void FileHandler::setReason(const FileReason & reason) {
	if (_prev)
		_prev->setReason(reason);
	_reason = reason;
}
void FileHandler::setFile(IFile *file) { _file = file; }
const FileHandler::FileReason& FileHandler::getReason() const { return _reason; }
void	FileHandler::reset_chain()
{
	if (_prev)
		return ;
	_file = NULL;
	_reason = "";
}

/**
 * FilePathBuilder class implementation
 */
FilePathBuilder::FilePathBuilder(const std::string& root, const std::string &uri) : _root(root), _uri(uri) {}
FilePathBuilder::~FilePathBuilder() {}
IFile* FilePathBuilder::handle(const std::string& path)
{
	std::string rel_path = _root;

	if (_root.empty())
		rel_path = "/";

	if (!_root.empty() && _root[rel_path.size() - 1] == '/')
		rel_path = _root.substr(0, _root.size() - 1);

	if (_uri.empty() || (!path.empty() && path.find(_uri) == 0))
	{
		std::string uri_path = path.substr(_uri.size());
		if (!uri_path.empty() && uri_path[0] != '/')
			rel_path += "/";
		rel_path += uri_path;
	}
	else if (!path.empty())
	{
		if (path[0] != '/')
			rel_path += "/";
		rel_path += path;
	}

	return (FileHandler::handle(path));
}

/**
 * FileExistenceChecker class implementation
 */
FileExistenceChecker::FileExistenceChecker() {}
FileExistenceChecker::~FileExistenceChecker() {}
IFile* FileExistenceChecker::handle(const std::string& path)
{
	struct stat st;

	if (stat(path.c_str(), &st) != 0)
		setReason("File not found");
	return (FileHandler::handle(path));
}

/**
 * FilePermissionChecker class implementation
 */
FilePermissionChecker::FilePermissionChecker(int flags): _flags(flags) {}
FilePermissionChecker::~FilePermissionChecker() {}
IFile* FilePermissionChecker::handle(const std::string &path)
{
	if ((_flags & R_OK) && access(path.c_str(), R_OK))
		setReason("No read permission");
	if ((_flags & W_OK) && access(path.c_str(), W_OK))
		setReason("No write permission");
	if ((_flags & X_OK) && access(path.c_str(), X_OK))
		setReason("No execute permission");
	return (FileHandler::handle(path));
}


/**
 * FileOpener class implementation
 */
FileOpener::FileOpener() {}
FileOpener::~FileOpener() {}
IFile*	FileOpener::handle(const std::string &path)
{
	if (_file)
		delete _file;
	_file = FileFactory::create(path, O_RDONLY);
	if (!_file)
		setReason("Failed to open file");
	return (FileHandler::handle(path));
}


