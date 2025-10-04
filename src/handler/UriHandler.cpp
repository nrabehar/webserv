#include "webserv.hpp"

using namespace Handler;

UriHandler::UriHandler(const std::string & uri, const LocationConfig * loc, RequestHandler * handler)
	: _uri(uri), _loc(loc), _handler(handler)
{

	size_t pos = _uri.find('?');
	if (pos != std::string::npos)
		_uri = _uri.substr(0, pos);

}

UriHandler::~UriHandler() {}

const std::string & UriHandler::uri() const { return (_uri); }

std::string	UriHandler::buildPath()
{

	std::string path = _loc->root;

	if (path[path.size() - 1] == '/')
		path = path.substr(0, path.size() - 1);
	
	std::string loc_path = _loc->path;
	if (loc_path[loc_path.size() - 1] == '/')
		loc_path = loc_path.substr(0, loc_path.size() - 1);

	if (loc_path != "/" && _uri.find(loc_path) == 0)
		_uri = _uri.substr(loc_path.size());

	if (!_uri.empty() && _uri[0] != '/')
		path += '/';

	path += _uri;

	if (!fileExists(path))
		_handler->setStatus(HS_NOT_FOUND);
	else if (isDirectory(path))
		getIndexPath(path);
	else if (!isReadable(path))
		_handler->setStatus(HS_FORBIDDEN);
	else if (isCgiPath(path))
		_handler->setStatus(HS_CGI);

	return (path);

}

bool	UriHandler::fileExists(const std::string & path) const
{

	struct stat st;
	return (stat(path.c_str(), &st) == 0);

}

bool	UriHandler::isDirectory(const std::string & path) const
{

	struct stat st;
	if (stat(path.c_str(), &st) != 0)
		return (false);
	return (S_ISDIR(st.st_mode));

}

bool	UriHandler::isReadable(const std::string & path) const
{

	return (access(path.c_str(), R_OK) == 0);

}

void	UriHandler::getIndexPath(std::string & path)
{

	if (_loc->index.empty())
	{
		if (_loc->autoindex)
			_handler->setStatus(HS_FOLDER_LISTING);
		else
			_handler->setStatus(HS_FORBIDDEN);
		return ;
	}

	if (path[path.size() - 1] != '/')
		path += '/';

	std::string og_path = path;
	for (size_t i = 0; i < _loc->index.size(); ++i)
	{

		std::string index_path = path + _loc->index[i];
		if (!fileExists(index_path) || isDirectory(index_path) || !isReadable(index_path))
			continue ;

		path = index_path;
		break ;

	}
	if (path != og_path)
		return ;
	if (_loc->autoindex)
		_handler->setStatus(HS_FOLDER_LISTING);
	else
		_handler->setStatus(HS_FORBIDDEN);

}

bool UriHandler::isCgiPath(const std::string & path) const
{

	if (_loc->cgi.empty())
		return (false);

	for (size_t i = 0; i < _loc->cgi.size(); ++i)
	{
		const std::string & ext = _loc->cgi[i].first;
		if (path.length() <= ext.length())
			continue ;
		if (path.compare(path.length() - ext.length(), ext.length(), ext))
			continue ;
		return (true);
	}
	return (false);

}
