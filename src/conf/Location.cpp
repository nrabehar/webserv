#include "Config.hpp"

Location::Location()
	: _autoIndex(false), _hasRedirect(false), _maxBodySize(0) {}
Location::~Location() {}

Location::Location(const Location &src)
		: _path(src._path), _root(src._root), _cgi(src._cgi),
			_uploadPath(src._uploadPath), _autoIndex(src._autoIndex),
			_hasRedirect(src._hasRedirect), _maxBodySize(src._maxBodySize),
			_indexs(src._indexs), _methods(src._methods), _redirect(src._redirect) {}

const std::string &Location::getPath() const { return _path; }
const std::string &Location::getRoot() const { return _root; }
const std::string &Location::getCgi() const { return _cgi; }
const std::string &Location::getUploadPath() const { return _uploadPath; }
const std::vector<std::string> &Location::getIndexs() const { return _indexs; }
const std::vector<std::string> &Location::getMethods() const { return _methods; }
const std::pair<int, std::string> &Location::getRedirect() const { return _redirect; }
bool Location::getAutoIndex() const { return _autoIndex; }
bool Location::hasRedirect() const { return _hasRedirect; }
size_t Location::getMaxBodySize() const { return _maxBodySize; }

void Location::setPath(const std::string &path) { _path = path; }
void Location::setRoot(const std::string &root) { _root = root; }
void Location::setCgi(const std::string &cgi) { _cgi = cgi; }
void Location::setUploadPath(const std::string &path) { _uploadPath = path; }
void Location::setIndexs(std::vector<std::string> &index) { _indexs = index; }
void Location::setMethods(std::vector<std::string> &method) { _methods = method; }
void Location::setRedirect(int code, const std::string &uri)
{
	_redirect = std::make_pair(code, uri);
	_hasRedirect = true;
}
void Location::setAutoIndex(bool autoIndex) { _autoIndex = autoIndex; }
void Location::setMaxBodySize(size_t size) { _maxBodySize = size; }

bool Location::isAllowedMethod(const std::string &method) const
{
	std::vector<std::string>::const_iterator it;

	it = std::find(_methods.begin(), _methods.end(), method);
	return (it != _methods.end());
}

void Location::setProperty(const std::string &key, const std::string &value)
{
	if (key == "root")
		_root = value;
	else if (key == "index")
		_indexs = splitStr(value, ' ');
	else if (key == "method")
		_methods = splitStr(value, ' ');
	else if (key == "autoindex")
		_autoIndex = value == "on";
	else if (key == "cgi")
		_cgi = value;
	else if (key == "upload")
		_uploadPath = value;
	else if (key == "return")
	{
		std::vector<std::string> parts = splitStr(value, ' ');
		if (parts.size() == 2)
		{
			if (!isNumeric(parts[0]))
				throw ParseException("Invalid redirect code: " + parts[0]);
			_redirect = std::make_pair(std::atoi(parts[0].c_str()), parts[1]);
			_hasRedirect = true;
		}
		else
			throw ParseException("Invalid redirect arguments: " + value);
	}
	else
		throw ParseException("Unknown location key: " + key);
}
