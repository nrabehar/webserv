#include "webserv.hpp"

/**
 * FileHandler base class implementation
 */

FileHandler::FileHandler() : _next(NULL), _reason("") {}
FileHandler::~FileHandler() {}
IFileHandler* FileHandler::setNext(IFileHandler* next) {
	_next = next;
	return next;
}
IFile* FileHandler::handle(const std::string& path) {
	if (_next && _reason.empty()) {
		return _next->handle(path);
	}
	return NULL;
}
void FileHandler::setReason(const FileReason & reason) { _reason = reason; }
const FileHandler::FileReason& FileHandler::getReason() const { return _reason; }

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

	if (stat(path.c_str(), &st) == 0)
		return (FileHandler::handle(path));
	setReason("File not found");
	return (NULL);
}

/**
 * FilePermissionChecker class implementation
 */