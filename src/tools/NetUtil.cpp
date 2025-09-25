#include "webserv.hpp"

NetUtil::~NetUtil() {}

bool NetUtil::isValidPort(int port)
{ return (port > 0 && port < 65535); }

bool NetUtil::isValidPortStr(const std::string &p_str)
{

	if (!String::isNumeric(p_str))
		return (false);
	return (isValidPort(std::atoi(p_str.c_str())));

}

bool	NetUtil::isValidIp(const std::string & ip)
{

	if (ip == "localhost")
		return (true);
	std::vector<std::string> parts = String::split(ip, ".");
	if (parts.size() != 4)
		return (false);

	for (size_t i = 0; i < parts.size(); ++i)
	{

		if (!String::isNumeric(parts[i]))
			return (false);
		int octet = std::atoi(parts[i].c_str());
		if (octet < 0 || octet > 255)
			return (false);

	}

	return (true);

}
