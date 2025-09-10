#include "webserv.hpp"

Location::Location():
	_autoindex(false),
	_uri(""),
	_root(""),
	_max_body_size(0),
	_upload_path(""),
	_index(),
	_method(),
	_redirect(),
	_cgi() {
}

Location::~Location() {
}

Location::Location(const Location & src) {
	(*this) = src;
}

Location & Location::operator=(const Location & src)
{
	if (this != &src) {
		_autoindex = src._autoindex;
		_uri = src._uri;
		_root = src._root;
		_max_body_size = src._max_body_size;
		_upload_path = src._upload_path;
		_index = src._index;
		_method = src._method;
		_redirect = src._redirect;
		_cgi = src._cgi;
	}
	return (*this);
}

void Location::parse(const std::string &)
{
	// @todo Implementation of the parse function
}

EErrorCode Location::check() const
{
	// @todo Implementation of the check function
	return (ST_OK);
}

void Location::reportError(EErrorCode code)
{
	switch (code)
	{
		default:
			break;
	}
}

bool Location::getAutoindex() const { return (_autoindex); }
size_t Location::getMaxBodySize() const { return (_max_body_size); }
const std::string & Location::getUri() const { return (_uri); }
const std::string & Location::getRoot() const { return (_root); }
const std::string & Location::getUploadPath() const { return (_upload_path); }
const std::vector<std::string>& Location::getIndex() const { return (_index); }
const std::vector<std::string>& Location::getMethod() const { return (_method); }
const std::map<EStatusCode, std::string> & Location::getRedirect() const { return (_redirect); }
const std::vector<std::pair<std::string, std::string> > & Location::getCgi() const { return (_cgi); }
