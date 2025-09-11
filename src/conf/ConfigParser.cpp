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
		server.parse(_server_block[i]);
		_server.push_back(server);
		std::cout << "Server "<< i + 1 << " parsed with " << server.getLocation().size() << " locations\n";
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
		if (!isValidIp(entry.addr))
			throw std::runtime_error("Configuration: Invalid IP address in listen directive: " + entry.addr);
		if (entry.addr[0] == '[' && entry.addr[entry.addr.size() - 1] == ']')
		{
			entry.addr = entry.addr.substr(1, entry.addr.size() - 2);
			entry.is_ipv6 = true;
		}
		if (String::isNumber(port_str))
		{
			entry.port = std::atoi(port_str.c_str());
		}
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

bool ConfigParser::isValidIp(const std::string &ip)
{
	if (ip.empty())
		return false;

	if (ip[0] == '[')
	{
		if (ip[ip.length() - 1] != ']')
			return false;
		std::string ipv6 = ip.substr(1, ip.length() - 2);
		size_t colon_count = 0;
		for (size_t i = 0; i < ipv6.length(); ++i)
		{
			if (ipv6[i] == ':')
				colon_count++;
			else if (!isxdigit(ipv6[i]))
				return false;
		}
		return (colon_count >= 2 && colon_count <= 7);
	}
	size_t dot_count = 0;
	size_t len = ip.length();
	for (size_t i = 0; i < len; ++i)
	{
		if (ip[i] == '.')
		{
			dot_count++;
			if (i == 0 || i == len - 1 || ip[i - 1] == '.' || ip[i + 1] == '.')
				return false;
		}
		else if (!isdigit(ip[i]))
			return false;
	}
	if (dot_count != 3)
		return false;
	std::vector<std::string> octets = String::split(ip, ".");
	if (octets.size() != 4)
		return false;

	for (size_t i = 0; i < octets.size(); ++i)
	{
		if (octets[i].empty() || (octets[i].length() > 1 && octets[i][0] == '0'))
			return false;
		int octet = std::atoi(octets[i].c_str());
		if (octet < 0 || octet > 255)
			return false;
	}

	return true;
}

bool ConfigParser::isValidPort(int port)
{
	return (port >= 0 && port <= 65535);
}