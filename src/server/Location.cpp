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

void Location::parse(const std::string &block)
{
	std::istringstream stream(block);
	std::string token;
	int brace_count = 0;

	if (stream >> token)
	  _uri = token;
	else
		throw std::runtime_error("Location::parse: missing URI in location block");
	if (!(stream >> token) || token != "{")
		throw std::runtime_error("Location::parse: expected '{' after URI in location block");
	brace_count = 1;
	while (stream >> token)
	{
		if (token == "{")
			brace_count++;
		else if (token == "}")
			brace_count--;
		if (brace_count < 0)
			throw std::runtime_error("Location::parse: unexpected closing brace '}'");
		if (brace_count == 0)
			break;
		std::string line;
		if (std::getline(stream, line))
		{
			line = String::trim(line, " \t;");
			if (line.empty())
			 continue;
			token = String::toLower(token);
			setDirective(token, line);
		}
		else
		{
			throw std::runtime_error("Location::parse: unexpected end of line after '" + token + "'");
		}
	}
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
const std::vector<CgiLink> & Location::getCgi() const { return (_cgi); }

void Location::setAutoindex(bool autoindex) { _autoindex = autoindex; }
void Location::setMaxBodySize(size_t max_body_size) { _max_body_size = max_body_size; }
void Location::setRoot(const std::string & root) { _root = root; }

void Location::setDirective(const std::string &directive, const std::string &value)
{
	if (directive == "autoindex")
		_autoindex = (String::toLower(value) == "on");
	else if (directive == "root")
		_root = value;
	else if (directive == "index")
		_index = String::split(value, " \t");
	else if (directive == "upload_path")
		_upload_path = value;
	else if (directive == "max_body_size" || directive == "client_max_body_size")
		_max_body_size = String::toBytes(value);
	else if (directive == "allow_methods" || directive == "methods")
		_method = String::split(String::toUpper(value), " \t");
	else if (directive == "return")
		_redirect = ConfigParser::parseRedirect(value);
	else if (directive == "cgi")
		_cgi.push_back(ConfigParser::parseCgi(value));
}