#include "webserv.hpp"

ConfigParser::ConfigParser(const AFile &file) : _file(file),
																								_server(),
																								_server_block()
{
	try
	{
		_server_block = Block::extractServer(_file.getContent());
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}

ConfigParser::~ConfigParser()
{
}

void ConfigParser::parse(const std::string &contents = "")
{
	(void)contents;
	for (size_t i = 0; i < _server_block.size(); i++)
	{
		Server server;
		try
		{
			server.parse(_server_block[i]);
			_server.push_back(server);
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

EErrorCode ConfigParser::check() const
{
	return (ST_OK);
}

void ConfigParser::reportError(EErrorCode)
{
}

const std::vector<Server> &ConfigParser::getServer() const { return (_server); }
const std::vector<std::string> &ConfigParser::getServerBlock() const { return (_server_block); }

AddrPort ConfigParser::parseHostPort(const std::string &token)
{
	AddrPort entry;

	if (token.empty())
		throw std::runtime_error("Configuration: Empty listen directive");
	size_t pos = token.find_last_of(':');
	if (pos != std::string::npos)
	{
		entry.addr = token.substr(0, pos);
		std::string port_str = token.substr(pos + 1);
		if (entry.addr[0] == '[' && entry.addr[entry.addr.size() - 1] == ']')
		{
			entry.addr = entry.addr.substr(1, entry.addr.size() - 2);
			entry.is_ipv6 = true;
		}
		if (String::isNumber(port_str))
			entry.port = std::atoi(port_str.c_str());
		else if (!port_str.empty())
			throw std::runtime_error("Configuration: Invalid port number in listen directive: " + port_str);
		else
			entry.port = 80;
	}
	else if (String::isNumber(token))
	{
		entry.port = std::atoi(token.c_str());
		entry.addr = "0.0.0.0";
	}
	return (entry);
}

std::map<EStatusCode, std::string> ConfigParser::parseErrorPage(const std::string &value)
{
	std::map<EStatusCode, std::string> error_map;
	std::vector<std::string> tokens = String::split(value, " \t");
	if (tokens.size() < 2)
	{
		std::cout << "Tokens size: " << tokens.size() << std::endl;
		throw std::runtime_error("Configuration: Invalid error_page directive: " + value);
	}
	std::string path = tokens.back();
	for (size_t i = 0; i < tokens.size() - 1; i++)
	{
		if (String::isNumber(tokens[i]))
		{
			int code = std::atoi(tokens[i].c_str());
			if (code >= 400 && code < 600)
				error_map[static_cast<EStatusCode>(code)] = path;
			else
				throw std::runtime_error("Configuration: Invalid status code in error_page directive: " + tokens[i]);
		}
		else
		{
			throw std::runtime_error("Configuration: Non-numeric status code in error_page directive: " + tokens[i]);
		}
	}
	return (error_map);
}

std::map<EStatusCode, std::string> ConfigParser::parseRedirect(const std::string &value)
{
	std::map<EStatusCode, std::string> redirect_map;
	std::vector<std::string> tokens = String::split(value, " \t");
	if (tokens.size() != 2)
	{
		throw std::runtime_error("Configuration: Invalid redirect directive: " + value);
	}
	if (String::isNumber(tokens[0]))
	{
		int code = std::atoi(tokens[0].c_str());
		if (code >= 300 && code < 400)
			redirect_map[static_cast<EStatusCode>(code)] = tokens[1];
		else
			throw std::runtime_error("Configuration: Invalid status code in redirect directive: " + tokens[0]);
	}
	else
	{
		throw std::runtime_error("Configuration: Non-numeric status code in redirect directive: " + tokens[0]);
	}
	return (redirect_map);
}

CgiLink ConfigParser::parseCgi(const std::string &value)
{
	std::vector<std::string> tokens = String::split(value, " \t");
	if (tokens.size() != 2)
	{
		throw std::runtime_error("Configuration: Invalid cgi directive: " + value);
	}
	CgiLink cgi;
	cgi.extension = tokens[0];
	cgi.path = tokens[1];
	return (cgi);
}
