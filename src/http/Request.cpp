#include "webserv.hpp"

using namespace Http;

Request::Request()
 : _method("GET"),
	 _uri("/"),
	 _version("HTTP/1.1"),
	 _headers(),
	 _body()
{}

Request & Request::operator=(const Request & other)
{
	if (this != &other)
	{
		this->_method = other._method;
		this->_uri = other._uri;
		this->_version = other._version;
		this->_headers = other._headers;
		this->_body = other._body;
	}
	return (*this);
}

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

const std::string & Request::body() const
{
	return (_raw_body);
}

const std::vector<RequestBody> & Request::bodyFields() const
{
	return (_body);
}

void Request::appendBody(const std::string & body)
{
	_raw_body += body;
}

void Request::addBodyField(const std::string & field, const std::string & filename, const std::string & value)
{
	RequestBody rb;
	rb.field = field;
	rb.filename = filename;
	rb.value = value;
	_body.push_back(rb);
}

size_t	Request::contentLength() const { return (_ct_len); }
void	Request::setContentLength(size_t cl) { _ct_len = cl; }

const std::string &	Request::contentType() const { return (_ct_type); }
void	Request::setContentType(const std::string & ct) { _ct_type = ct; }
