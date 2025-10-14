#include "webserv.hpp"

using namespace Http;

Parser::Parser(Handler::RequestHandler * req_handler)
	: _state(REQUEST_LINE), _mp_state(BS_BOUNDARY), _body_type(RAW),
		_field(), _filename(), _value(), _tmp_filename(), _tmp_file(NULL),
		_req_handler(req_handler), _is_cgi(false)
{
}

Parser::~Parser()
{
}

void Parser::setState(State state)
{
	_state = state;
}

Parser::ParseState Parser::state() const
{

	if (_state == DONE)
		return (PS_DONE);
	if (_state == ERROR || _mp_state == BS_ERROR)
		return (PS_ERROR);
	return (PS_INCOMPLETE);
}

void Parser::reset()
{

	if (_state == DONE)
		_state = REQUEST_LINE;
	_mp_state = BS_BOUNDARY;
	_body_type = RAW;
	_field.clear();
	_filename.clear();
	_value.clear();
	_tmp_filename.clear();
	_is_cgi = false;
	if (_tmp_file)
	{
		delete _tmp_file;
		_tmp_file = NULL;
	}

}

bool Parser::parseNext(Buffer & buf, Request & req, Response & res)
{
	if (state() == PS_DONE)
		return (false);
	if (state() == PS_ERROR || _req_handler->isError())
		return (waitForRecycle(buf, req));
	if (_state == REQUEST_LINE)
		req = Request();
	bool ret = true;
	while (ret && !_req_handler->isError())
	{
		switch (_state)
		{
			case REQUEST_LINE:
				ret = parseReqLine(buf, req);
				break;
			case HEADERS:
				ret = parseHeaders(buf, req, res);
				break;
			case BODY:
				ret = parseBody(buf, req);
				break;
			case DONE:
			case ERROR:
				ret = false;
				break;
		}
		if (_state == BODY)
			ret = false;
	}
	if (!ret)
	{
		if (_state == BODY)
			ret = true;
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
		_req_handler->setStatus(Handler::HS_BAD_REQUEST);
		return (false);
	}

	std::string line(ptr, line_len - 2);
	std::vector<std::string> parts = String::split(line);
	if (parts.size() != 3 || parts[2].substr(0, 5) != "HTTP/"
	|| parts[1][0] != '/')
	{
		_req_handler->setStatus(Handler::HS_BAD_REQUEST);
		return (false);
	}

	req.setMethod(parts[0]);
	req.setUri(parts[1]);
	req.setVersion(parts[2]);
	buf.hasRead(line_len);
	setState(HEADERS);
	_is_cgi = _req_handler->isCgiRequest(req);
	return (true);

}

bool Parser::parseHeaders(Buffer & buf, Request & req, Response & res)
{

	size_t	end = buf.find("\r\n\r\n");
	if (end == std::string::npos)
		return (true);

	std::string hdr = buf.substr(0, end);
	buf.hasRead(end + 4);
	std::string	line;
	std::istringstream iss(hdr);
	while (std::getline(iss, line))
	{
		line = String::trim(line);
		size_t	colon = line.find(':');
		if (colon == std::string::npos)
		{
			_req_handler->setStatus(Handler::HS_BAD_REQUEST);
			return (false);
		}
		std::string key = String::toCamelCase(String::trim(line.substr(0, colon)), '-');
		std::string value = String::trim(line.substr(colon + 1));
		req.setHeader(key, value);
		if (key == "Content-Type")
		{
			req.setContentType(value);
			if (value.find("multipart/form-data") == 0)
			{
				_boundary = getBoundary(value);
				_body_type = MULTIPART;
			}
			else if (value.find("application/x-www-form-urlencoded") == 0)
				_body_type = URLENCODED;
		}
		else if (key == "Content-Length")
			req.setContentLength(static_cast<size_t>(std::atoi(value.c_str())));
		else if (key == "Transfer-Encoding" && value.find("chunked") != std::string::npos)
			_body_type = CHUNKED;
	}
	const LocationConfig * loc = _req_handler->findLocation(req.uri());
	if (!loc->allowsMethod(req.method()))
	{
		_req_handler->setStatus(Handler::HS_METHOD_NOT_ALLOWED);
		if (req.method() == "POST")
			setState(ERROR);
		return (false);
	}
	if (req.uri().size() >= 2048)
	{
		_req_handler->setStatus(Handler::HS_URI_TOO_LONG);
		if (req.method() == "POST")
			setState(ERROR);
		return (false);
	}
	if (req.contentLength() > loc->client_max_body_size)
	{
		setState(ERROR);
		_req_handler->setStatus(Handler::HS_REQUEST_ENTITY_TOO_LARGE);
		return (false);
	}
	if (_is_cgi)
	{
		if (!_req_handler->initCgiHandler(req, res))
		{
			_req_handler->setStatus(Handler::HS_BAD_REQUEST);
			return (false);
		}
	}
	if (req.method() == "POST")
	{
		setState(BODY);
		return (true);
	}
	if (_is_cgi)
		_req_handler->cgiHandler()->closeIn(Handler::HS_OK);
	setState(DONE);
	return (false);

}

bool Parser::parseBody(Buffer & buf, Request & req)
{

	switch (_body_type)
	{
		case MULTIPART:
			return (parseMultiPartBody(buf, req));
		case URLENCODED:
			return (parseUrlEncoded(buf, req));
		case CHUNKED:
			return (parseChunkedBody(buf, req));
		default:
		{
			req.appendBody(std::string(buf.readPtr(), buf.readable()));
			buf.hasRead(buf.readable());
			return (buf.readable());
		}
	}
	
}

bool Parser::parseUrlEncoded(Buffer & buf, Request & req)
{

	if (req.body().size() < req.contentLength())
	{
		req.appendBody(std::string(buf.readPtr(), buf.readable()));
		if (_is_cgi)
			_req_handler->cgiHandler()->write(buf.readPtr(), buf.readable());
		buf.hasRead(buf.readable());
		return (true);
	}
	if (_is_cgi)
	{
		setState(DONE);
		_req_handler->cgiHandler()->closeIn(Handler::HS_OK);
		return (false);
	}
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

bool Parser::parseMultiPartBody(Buffer & buf, Request & req)
{

	if (_is_cgi)
	{
		_req_handler->cgiHandler()->write(buf.readPtr(), buf.readable());
		if (buf.find(_boundary + "--") != std::string::npos)
		{
			buf.hasRead(buf.readable());
			setState(DONE);
			return (false);
		}
		buf.hasRead(buf.readable());
		return (true);
	}

	while (buf.readable() && !_req_handler->isError())
	{
		switch (_mp_state)
		{
			case BS_BOUNDARY:
			{
				if (bodyMultipartBoundary(buf, req))
					return (true);
				break;
			}
			case BS_HEADER:
			{
				if (bodyMultipartHeader(buf, req))
					return (true);
				break ;
			}
			case BS_PART:
			{
				if (bodyMultipartPart(buf, req))
					return (true);
				break ;
			}
			default:
				break;
		}
	}
	return (true);

}

bool Parser::parseChunkedBody(Buffer & buf, Request & req)
{

	while (buf.readable())
	{

		size_t pos = buf.find("\r\n");
		if (pos == std::string::npos)
			return (true);
		std::string line = buf.substr(0, pos);
		buf.hasRead(pos + 2);
		size_t chunk_size = 0;
		std::istringstream iss(line);
		iss >> std::hex >> chunk_size;
		if (chunk_size == 0)
		{
			setState(DONE);
			buf.hasRead(buf.readable());
			if (_is_cgi)
				_req_handler->cgiHandler()->closeIn(Handler::HS_OK);
			return (false);
		}
		if (buf.readable() < chunk_size + 2)
			return (true);
		if (_is_cgi)
			_req_handler->cgiHandler()->write(buf.readPtr(), chunk_size);
		else
			req.appendBody(std::string(buf.readPtr(), chunk_size));
		buf.hasRead(chunk_size + 2);
	
	}

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
		_req_handler->setStatus(Handler::HS_BAD_REQUEST);

	std::string hdr = part.substr(0, pos);
	std::string value = part.substr(pos + 4);
	
	std::string content_disp = hdr.substr(hdr.find("Content-Disposition:"));
	if (content_disp.empty())
		_req_handler->setStatus(Handler::HS_BAD_REQUEST);

	std::string name = parseDisposition(content_disp, "name");
	std::string filename = parseDisposition(content_disp, "filename");
	if (name.empty())
		_req_handler->setStatus(Handler::HS_BAD_REQUEST);

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
			result += ' ';
		else
			result += str[i];
	}
	return (result);

}

bool Parser::bodyMultipartBoundary(Buffer & buf, Request & req)
{

	(void)req;
	size_t	pos = buf.find(_boundary);
	if (pos == std::string::npos)
		return (true);
	if (pos >= 2)
	{
		_mp_state = BS_PART;
		return (false);
	}
	if (_tmp_file)
	{
		delete _tmp_file;
		_tmp_file = NULL;
	}
	buf.hasRead(pos + _boundary.size());
	if (buf.readable() >= 2 && buf.readPtr()[0] == '-' && buf.readPtr()[1] == '-')
	{
		buf.hasRead(buf.readable());
		setState(DONE);
		_mp_state = BS_DONE;
		if (_is_cgi)
			_req_handler->cgiHandler()->closeIn(Handler::HS_OK);
		return (false);
	}
	_mp_state = BS_HEADER;
	return (false);

}

bool Parser::bodyMultipartHeader(Buffer & buf, Request & req)
{

	size_t	header_end = buf.find("\r\n\r\n");
	if (header_end == std::string::npos)
	{
		WRN("Could not find end of multipart header yet");
		return (true);
	}
	std::string hdr = buf.substr(0, header_end);
	buf.hasRead(header_end + 4);
	size_t ct_disp = hdr.find("Content-Disposition");
	if (ct_disp == std::string::npos)
	{
		_mp_state = BS_ERROR;
		ERR("Could not find Content-Disposition in multipart header");
		_req_handler->setStatus(Handler::HS_BAD_REQUEST);
		return (false);
	}
	hdr = hdr.substr(ct_disp);
	_field = parseDisposition(hdr, "name");
	_filename = parseDisposition(hdr, "filename");
	if (!_filename.empty())
	{
		const LocationConfig * loc = _req_handler->findLocation(req.uri());
		if (loc->upload_store.empty())
		{
			_req_handler->setStatus(Handler::HS_FORBIDDEN);
			setState(ERROR);
			_mp_state = BS_ERROR;
			return (false);
		}
		_tmp_filename = loc->upload_store + _filename;
		_tmp_file = FileFactory::create(_tmp_filename, O_CREAT | O_RDWR);
		if (!_tmp_file)
		{
			_mp_state = BS_ERROR;
			ERR("Could not upload: " + _filename);
			_req_handler->setStatus(Handler::HS_INTERNAL_SERVER_ERROR);
			return (false);
		}
		req.addBodyField(_field, _filename, _tmp_filename);
		_field = "";
		_filename = "";
	}
	_mp_state = BS_PART;
	return (false);

}

bool Parser::bodyMultipartPart(Buffer & buf, Request & req)
{

	size_t	b_pos = buf.find("\r\n" + _boundary);
	if (b_pos != std::string::npos)
	{
		_mp_state = BS_BOUNDARY;
		if (b_pos == 0)
		{
			buf.hasRead(2);
			return (false);
		}
	}
	if (_tmp_file)
	{
		std::string	data = buf.substr(0, b_pos);
		size_t	cap = std::min(b_pos, buf.readable());
		ssize_t n = _tmp_file->write(buf.readPtr(), cap);
		if (n <= 0)
		{
			_mp_state = BS_ERROR;
			_req_handler->setStatus(Handler::HS_INTERNAL_SERVER_ERROR);
			delete _tmp_file;
			_tmp_file = NULL;
			if (!_tmp_filename.empty() && access(_tmp_filename.c_str(), R_OK))
				std::remove(_tmp_filename.c_str());
			return (false);
		}
		buf.hasRead(n);
	}
	else
	{
		if (b_pos == std::string::npos)
			return (true);

		std::string val = String::trim(std::string(buf.readPtr(), b_pos));
		req.addBodyField(_field, _filename, parsePercentEncoding(val));
		buf.hasRead(buf.readable());
	}
	return (false);

}

bool	Parser::waitForRecycle(Buffer & buf, Request & req)
{

	std::string c_type = req.contentType();
	std::string te = req.header("Transfer-Encoding");

	if (c_type == "application/x-www-form-urlencoded" || c_type.find("multipart/form-data") == 0)
	{

		static ssize_t remain = 0;
		if (remain <= 0)
			remain = req.contentLength();
		remain -= buf.readable();
		buf.hasRead(buf.readable());
		if (remain <= 0)
		{
			setState(REQUEST_LINE);
			req = Request();
			return (false);
		}
	}
	if (te == "chunked")
	{
		if (buf.find("0\r\n\r\n") != std::string::npos)
		{
			buf.hasRead(buf.readable());
			setState(REQUEST_LINE);
			req = Request();
			return (false);
		}
		buf.hasRead(buf.readable());
	}
	return (true);

}
