#include "webserv.hpp"

using namespace Http;

Parser::Parser() : _state(REQUEST_LINE), _usr_state(PS_INCOMPLETE)
{
}

Parser::~Parser()
{
}

void Parser::setState(State state)
{
	_state = state;
	if (state == DONE)
		_usr_state = PS_DONE;
	else if (state == ERROR)
		_usr_state = PS_ERROR;
}

Parser::ParseState Parser::state() const
{
	return (_usr_state);
}

void Parser::reset()
{

	_state = REQUEST_LINE;
	_usr_state = PS_INCOMPLETE;

}

bool Parser::parseNext(Buffer & buf, Request & req)
{
	if (_usr_state == PS_DONE || _usr_state == PS_ERROR)
		return (false);

	bool ret = true;
	while (ret && _usr_state == PS_INCOMPLETE && buf.readable() > 0)
	{
		switch (_state)
		{
			case REQUEST_LINE:
				ret = parseReqLine(buf, req);
				break;
			case HEADERS:
				ret = parseHeaders(buf, req);
				break;
			case BODY:
				ret = parseBody(buf, req);
				break;
			case DONE:
			case ERROR:
				ret = false;
				break;
		}
	}
	return (ret);

}

bool Parser::parseReqLine(Buffer & buf, Request & req)
{

	const char *ptr = buf.readPtr();
	size_t len = buf.readable();

	const char *end = static_cast<const char *>(std::memchr(ptr, '\n', len));
	if (!end)
		return (true);

	size_t line_len = end - ptr + 1;
	if (line_len < 2 || *(end - 1) != '\r')
	{
		setState(ERROR);
		return (false);
	}

	std::string line(ptr, line_len - 2);
	std::vector<std::string> parts = String::split(line);
	if (parts.size() != 3 || parts[2].substr(0, 5) != "HTTP/")
	{
		setState(ERROR);
		return (false);
	}

	req.setMethod(parts[0]);
	req.setUri(parts[1]);
	req.setVersion(parts[2]);

	buf.hasRead(line_len);
	setState(HEADERS);
	return (true);

}

bool Parser::parseHeaders(Buffer & buf, Request & req)
{

	const char *ptr = buf.readPtr();
	size_t len = buf.readable();

	const char *end = static_cast<const char *>(std::memchr(ptr, '\n', len));
	if (!end)
		return (true);

	size_t line_len = end - ptr + 1;
	if (line_len < 2 || *(end - 1) != '\r')
	{
		setState(ERROR);
		return (false);
	}

	if (line_len == 2)
	{
		buf.hasRead(line_len);
		setState(BODY);
		return (false);
	}

	std::string line(ptr, line_len - 2);
	size_t colon = line.find(':');
	if (colon == std::string::npos)
	{
		setState(ERROR);
		return (false);
	}

	//TODO transform to camel case keys like Content-Length
	std::string key = String::trim(line.substr(0, colon));
	//TODO transform to lower case values like text/html
	std::string value = String::trim(line.substr(colon + 1));
	req.setHeader(key, value);

	buf.hasRead(line_len);

	return (true);

}

bool Parser::parseBody(Buffer & buf, Request & req)
{

	// TODO handle Transfer-Encoding: chunked
	if (req.header().count("Content-Length") == 0)
	{
		setState(DONE);
		return (false);
	}
	const std::string &cl = req.header("Content-Length");
	if (!String::isNumeric(cl))
	{
		setState(ERROR);
		return (false);
	}
	size_t content_length = std::strtoul(cl.c_str(), NULL, 10);
	size_t len = buf.readable();
	if (len < content_length)
		return (true);
	setState(DONE);
	return (false);

}
