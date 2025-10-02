#include "webserv.hpp"

using namespace Http;

Response::Response()
: _status(200), _version("HTTP/1.1"), _reason(), _headers(), _body()
{}

Response::Response(int status, const std::string & reason)
: _status(status), _version("HTTP/1.1"), _reason(reason), _headers(), _body()
{}

Response & Response::operator=(const Response & other)
{
	if (this != &other)
	{
		this->_status = other._status;
		this->_version = other._version;
		this->_reason = other._reason;
		this->_headers = other._headers;
		this->_body = other._body;
	}
	return (*this);
}

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

const std::string & Response::body() const
{
	return (this->_body);
}

void Response::appendBody(const std::string & body)
{
	this->_body += body;
}

const std::string & Response::reason() const
{
	return (this->_reason);
}

void Response::setReason(const std::string & reason)
{
	this->_reason = reason;
}

std::string Response::str() const
{

	std::ostringstream oss;

	oss << this->_version << " " << this->_status;
	if (!this->_reason.empty())
		oss << " " << this->_reason;
	else
		oss << " " << "OK";
	oss << "\r\n";

	std::map<std::string, std::string>::const_iterator it;
	for (it = this->_headers.begin(); it != this->_headers.end(); ++it)
	{
		oss << it->first << ": " << it->second << "\r\n";
	}
	oss << "\r\n";
	oss << this->_body;

	return (oss.str());

}
