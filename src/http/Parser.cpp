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

	_state = REQUEST_LINE;
	_mp_state = BS_BOUNDARY;
	_body_type = RAW;
	_field.clear();
	_filename.clear();
	_value.clear();
	_tmp_filename.clear();
	if (_tmp_file)
	{
		delete _tmp_file;
		_tmp_file = NULL;
	}

}

bool Parser::parseNext(Buffer & buf, Request & req, Response & res)
{
	if (state() == PS_DONE || state() == PS_ERROR)
		return (false);

	bool ret = true;
	while (ret)
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
	_is_cgi = _req_handler->isCgiRequest(req);
	return (true);

}

bool Parser::parseHeaders(Buffer & buf, Request & req, Response & res)
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
		if (_is_cgi)
			_req_handler->initCgiHandler(req, res);
		buf.hasRead(line_len);
		if (req.method() == "POST" )
		{
			setState(BODY);
			return (true);
		}
		setState(DONE);
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
	std::string value = String::trim(line.substr(colon + 1));
	req.setHeader(key, value);
	buf.hasRead(line_len);

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

	return (true);

}

bool Parser::parseBody(Buffer & buf, Request & req)
{

	LOG("Parsing body: " + String::str(_body_type));
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
		buf.hasRead(buf.readable());
		return (true);
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

	while (buf.readable())
	{
		switch (_mp_state)
		{
			case BS_BOUNDARY:
			{
				size_t	pos = buf.find(_boundary);
				if (pos == std::string::npos)
					return (true);
				if (pos >= 2)
				{
					LOG("Boundary found");
					_mp_state = BS_PART;
					break ;
				}
				if (_tmp_file)
				{
					delete _tmp_file;
					_tmp_file = NULL;
				}
				buf.hasRead(pos + _boundary.size());
				if (buf.readable() >= 2 && buf.readPtr()[0] == '-' && buf.readPtr()[1] == '-')
				{
					buf.hasRead(2); // End of boundary
					setState(DONE);
					_mp_state = BS_DONE;
					return (false);
				}
				_mp_state = BS_HEADER;
				break;
			}
			case BS_HEADER:
			{
				LOG("Pass at header");
				size_t	header_end = buf.find("\r\n\r\n");
				if (header_end == std::string::npos)
					return (true);
				std::string hdr = buf.substr(0, header_end);
				buf.hasRead(header_end + 4);
				size_t ct_disp = hdr.find("Content-Disposition");
				if (ct_disp == std::string::npos)
				{
					setState(ERROR);
					return (false);
				}
				hdr = hdr.substr(ct_disp);
				_field = parseDisposition(hdr, "name");
				_filename = parseDisposition(hdr, "filename");
				if (!_filename.empty())
				{
					_tmp_filename = "tmp/" + _filename + ".tmp";
					_tmp_file = FileFactory::create(_tmp_filename, O_CREAT | O_RDWR);
					if (!_tmp_file)
					{
						setState(ERROR);
						ERR("Could not upload: " + _filename);
						// set request status to 500;
						return (false);
					}
					req.addBodyField(_field, _filename, _tmp_filename);
					_field = "";
					_filename = "";
				}
				_mp_state = BS_PART;
				break ;
			}
			case BS_PART:
			{
				size_t	b_pos = buf.find("\r\n" + _boundary);
				if (b_pos != std::string::npos)
				{
					_mp_state = BS_BOUNDARY;
					if (b_pos == 0)
					{
						buf.hasRead(2);
						break ;
					}
				}
				if (_tmp_file)
				{
					LOG("We have: " + String::str(buf.readable()) + " bytes to write");
					std::string	data = buf.substr(0, b_pos);
					size_t	cap = std::min(b_pos, buf.readable());
					ssize_t n = _tmp_file->write(buf.readPtr(), cap);
					if (n <= 0)
					{
						// TODO: remove all created tmp files
						setState(ERROR);
						ERR("Could not write to temporary file: " + std::string(strerror(errno)));
						delete _tmp_file;
						_tmp_file = NULL;
						if (!_tmp_filename.empty() && access(_tmp_filename.c_str(), R_OK))
							std::remove(_tmp_filename.c_str());
						return (false);
					}
					LOG("We just written " + String::str(n) + " bytes into " +  _tmp_filename);
					LOG("Buffer read end size: " + String::str(buf.readable()));
					buf.hasRead(n);
					LOG("Buffer read end size after: " + String::str(buf.readable()));
				}
				else
				{
					if (b_pos == std::string::npos)
						return (true);

					std::string val = String::trim(std::string(buf.readPtr(), b_pos));
					req.addBodyField(_field, _filename, parsePercentEncoding(val));
					buf.hasRead(buf.readable());
				}
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
