#include "webserv.hpp"

using namespace Http;

Response::Response()
: _version("HTTP/1.1"), _status(200), _headers(), _body()
{}

Response::Response(int status, const std::string & body)
: _version("HTTP/1.1"), _status(status), _headers(), _body(body)
{}

Response::~Response()
{}

const std::string & Response::version() const
{
	return (this->_version);
}

void Response::setVersion(const std::string & version)
{
	this->_version = version;
}

int Response::statusCode() const
{
	return (this->_status);
}

void Response::setStatusCode(int status)
{
	this->_status = status;
}

const std::map<std::string, std::string> & Response::header() const
{
	return (this->_headers);
}

const std::string & Response::header(const std::string & key) const
{
	std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
	if (it != this->_headers.end())
		return (it->second);
	static const std::string empty;
	return (empty);
}

void Response::setHeader(const std::string & key, const std::string & value)
{
	this->_headers[key] = value;
}
