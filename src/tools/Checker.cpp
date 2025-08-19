#include "Checker.hpp"

bool isNumeric(const std::string &str)
{
	if (str.empty())
		return false;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isdigit(str[i]))
		{
			return false;
		}
	}
	return true;
}

bool isValidIP(const std::string &ip)
{
	std::vector<std::string> parts = splitStr(ip, '.');
	if (parts.size() != 4)
		return false;

	for (size_t i = 0; i < parts.size(); ++i)
	{
		if (!isNumeric(parts[i]))
			return false;
		int octet = std::atoi(parts[i].c_str());
		if (octet < 0 || octet > 255)
			return false;
	}
	return true;
}

bool isValidMethod(const std::string &method)
{
	return method == "GET" || method == "POST" || method == "DELETE";
}

bool isValidPort(size_t port)
{
	return port > 0 && port <= 65535;
}
