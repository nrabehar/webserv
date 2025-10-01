#include "webserv.hpp"

using namespace Http;

Request::Request()
 : _method("GET"),
	 _uri("/"),
	 _version("HTTP/1.1"),
	 _headers(),
	 _body()
{}

Request::~Request() {}

const std::string & Request::method() const
{
	return (_method);
}

void Request::setMethod(const std::string & method)
{
	_method = method;
}

const std::string & Request::uri() const
{
	return (_uri);
}

void Request::setUri(const std::string & uri)
{
	_uri = uri;
}

const std::string & Request::version() const
{
	return (_version);
}

void Request::setVersion(const std::string & version)
{
	_version = version;
}

const std::map<std::string, std::string> & Request::header() const
{
	return (_headers);
}

const std::string & Request::header(const std::string & key) const
{

	std::map<std::string, std::string>::const_iterator it = _headers.find(key);
	if (it != _headers.end())
		return (it->second);

	static const std::string empty = "";
	return (empty);

}

void Request::setHeader(const std::string & key, const std::string & value)
{
	_headers[key] = value;
}

