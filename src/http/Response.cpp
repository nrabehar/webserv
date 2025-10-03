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

int Response::status() const
{
	return (this->_status);
}

void Response::setStatus(int status)
{
	this->_status = status;
	_reason = reason(status);
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

std::string Response::reason(int status) const
{
	switch (status)
	{
		case 200: return ("OK");
		case 201: return ("Created");
		case 202: return ("Accepted");
		case 204: return ("No Content");
		case 301: return ("Moved Permanently");
		case 302: return ("Found");
		case 303: return ("See Other");
		case 304: return ("Not Modified");
		case 307: return ("Temporary Redirect");
		case 400: return ("Bad Request");
		case 401: return ("Unauthorized");
		case 403: return ("Forbidden");
		case 404: return ("Not Found");
		case 405: return ("Method Not Allowed");
		case 413: return ("Payload Too Large");
		case 414: return ("URI Too Long");
		case 500: return ("Internal Server Error");
		case 501: return ("Not Implemented");
		case 502: return ("Bad Gateway");
		case 503: return ("Service Unavailable");
		case 504: return ("Gateway Timeout");
		default: return ("Unknown Status");
	}
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
