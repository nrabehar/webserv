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
		throw std::runtime_error("Configuration: missing URI in location block");
	if (!(stream >> token) || token != "{")
		throw std::runtime_error("Configuration: expected '{' after URI in location block");
	brace_count = 1;
	while (stream >> token)
	{
		if (token == "{")
			brace_count++;
		else if (token == "}")
			brace_count--;
		if (brace_count < 0)
			throw std::runtime_error("Configuration: unexpected closing brace '}'");
		if (brace_count == 0)
			break;
		std::string line;
		if (std::getline(stream, line))
		{
			line = String::trim(line, " \t;");
			if (line.empty())
			 throw std::runtime_error("Configuration: unexpected end of line after '" + token + "'");
			token = String::toLower(token);
			setDirective(token, line);
		}
		else
		{
			throw std::runtime_error("Configuration: unexpected end of line after '" + token + "'");
		}
	}
	EErrorCode err = check();
	if (err != ST_OK)
		reportError(err);
}

EErrorCode Location::check() const
{
	if (_uri.empty() || _uri[0] != '/')
		return (ST_BAD_URI);

	for (size_t i = 0; i < _cgi.size(); ++i)
		if (_cgi[i].extension.empty() || _cgi[i].path.empty())
			return (ST_BAD_CGI);

	for (size_t i = 0; i < _method.size(); ++i)
	{
		std::string method = String::toUpper(_method[i]);
		if (method != "GET" && method != "POST"
				&& method != "DELETE" && method != "PUT"
				&& method != "PATCH" && method != "HEAD"
				&& method != "OPTIONS")
			return (ST_BAD_METHOD);
	}

	return (ST_OK);
}

void Location::reportError(EErrorCode code)
{
	switch (code)
	{
		case ST_BAD_URI:
			throw std::runtime_error("Configuration: Invalid URI in location block: " + _uri);
			break;
		case ST_BAD_CGI:
			throw std::runtime_error("Configuration: Invalid CGI configuration in location block: " + _uri);
			break;
		case ST_BAD_METHOD:
			throw std::runtime_error("Configuration: Invalid method in location block: " + _uri);
			break;
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
const std::map<EStatusCode, std::string> & Location::getErrorPage() const { return (_errorpage); }
const std::map<EStatusCode, std::string> & Location::getRedirect() const { return (_redirect); }
const std::vector<CgiLink> & Location::getCgi() const { return (_cgi); }

void Location::setAutoindex(bool autoindex) { _autoindex = autoindex; }
void Location::setMaxBodySize(size_t max_body_size) { _max_body_size = max_body_size; }
void Location::setRoot(const std::string & root) { _root = root; }
void Location::setIndex(const std::vector<std::string> & index) { _index = index; }
void Location::setErrorPage(const std::map<EStatusCode, std::string> & errorpage) { _errorpage = errorpage; }

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
	else
	 	throw std::runtime_error("Configuration: unknown directive '" + directive + "'");
	std::cout << "Set directive '" << directive << "' with value '" << value << "' in location '" << _uri << "'\n";
}