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

	std::string key = String::toCamelCase(String::trim(line.substr(0, colon)), '-');
	std::string value = String::toLower(String::trim(line.substr(colon + 1)));
	req.setHeader(key, value);

	buf.hasRead(line_len);

	return (true);

}

bool Parser::parseBody(Buffer & buf, Request & req)
{

	const std::string &cl = req.header("Content-Length");
	const std::string &te = req.header("Transfer-Encoding");

	if (!cl.empty() && !te.empty())
	{
		setState(ERROR);
		return false;
	}
	bool has_cl = !cl.empty() && String::isNumeric(cl);
	bool is_chunked = te == "chunked";

	if (buf.readable() == 0)
		return (true);
	if (is_chunked)
		return (parseChunkedBody(buf, req));
	if (!has_cl)
	{
		setState(DONE);
		return (false);
	}
	size_t content_len = static_cast<size_t>(std::atoi(cl.c_str()));
	if (buf.readable() > content_len)
	{
		setState(ERROR);
		return (false);
	}
	if (buf.readable() < content_len)
		return (true);
	req.appendBody(std::string(buf.readPtr(), buf.readable()));
	const std::string &ct = req.header("Content-Type");
	if (ct.find("application/x-www-form-urlencoded") == 0)
		return (parseUrlEncoded(req));
	else if (ct.find("multipart/form-data") == 0)
		return (parseMultiPartBody(req));
	else
	{
		buf.hasRead(content_len);
		setState(DONE);
		return (false);
	}
	
}

bool Parser::parseUrlEncoded(Request & req)
{

	std::string body = req.body();
	std::vector<std::string> pairs = String::split(body, "&");
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		size_t eq = pairs[i].find('=');
		if (eq == std::string::npos)
			continue ;
		std::string key = parsePercentEncoding(pairs[i].substr(0, eq));
		std::string value = parsePercentEncoding(pairs[i].substr(eq + 1));
		req.addBodyField(key, "", value);
	}
	setState(DONE);
	return (false);

}

bool Parser::parseMultiPartBody(Request & req)
{

	std::string boundary = getBoundary(req.header("Content-Type"));
	if (boundary.empty())
	{
		setState(ERROR);
		return (false);
	}
	std::string end_boundary = boundary + "--";
	const std::string & body = req.body();
	size_t start = body.find(boundary);
	size_t end = 0;
	while (start != std::string::npos && _state != ERROR)
	{
		start += boundary.length();
		end = body.find(boundary, start);
		if (end == std::string::npos)
			end = body.find(end_boundary, start);
		if (end == std::string::npos)
			break ;
		std::string part = body.substr(start, end - start);
		parseField(part, req);
		start = end;
	}
	if (_state == ERROR)
		return (false);
	setState(DONE);
	return (false);

}

bool Parser::parseChunkedBody(Buffer & buf, Request & req)
{

	(void)buf;
	(void)req;
	setState(DONE);
	return (false);

}

std::string Parser::getBoundary(const std::string & ct)
{

	size_t pos = ct.find("boundary=");
	if (pos == std::string::npos)
		return ("");

	std::string boundary = String::trim(ct.substr(pos + 9), " \t\"");

	return ("--" + boundary);

}

void Parser::parseField(const std::string & part, Request & req)
{

	size_t pos = part.find("\r\n\r\n");
	if (pos == std::string::npos)
		return (setState(ERROR));

	std::string hdr = part.substr(0, pos);
	std::string value = part.substr(pos + 4);
	
	std::string content_disp = hdr.substr(hdr.find("Content-Disposition:"));
	if (content_disp.empty())
		return (setState(ERROR));

	std::string name = parseDisposition(content_disp, "name");
	std::string filename = parseDisposition(content_disp, "filename");
	if (name.empty())
		return (setState(ERROR));

	req.addBodyField(name, filename, value);
	
}

std::string Parser::parseDisposition(const std::string & disp, const std::string & field)
{

	size_t pos = disp.find(field + "=");
	if (pos == std::string::npos)
		return ("");

	pos += field.length() + 1;
	size_t end = disp.find(';', pos);
	if (end == std::string::npos)
		end = disp.find("\r\n", pos);
	if (end == std::string::npos)
		end = disp.length();

	std::string value = String::trim(disp.substr(pos, end - pos), " \t\"");
	value = parsePercentEncoding(value);
	return (value);

}

std::string Parser::parsePercentEncoding(const std::string & str)
{

	std::string result;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '%' && i + 2 < str.length() && isxdigit(str[i + 1]) && isxdigit(str[i + 2]))
		{
			std::string hex = str.substr(i + 1, 2);
			char decoded_char = static_cast<char>(std::strtol(hex.c_str(), NULL, 16));
			result += decoded_char;
			i += 2;
		}
		else if (str[i] == '+')
		{
			result += ' ';
		}
		else
		{
			result += str[i];
		}
	}
	return (result);

}
