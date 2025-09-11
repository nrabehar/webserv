#include "webserv.hpp"

String::String() {}
String::~String() {}

std::string String::toLower(const std::string &str)
{
	std::string result = str;
	for (size_t i = 0; i < result.size(); ++i)
		result[i] = std::tolower(result[i]);
	return result;
}

std::string String::toUpper(const std::string &str)
{
	std::string result = str;
	for (size_t i = 0; i < result.size(); ++i)
		result[i] = std::toupper(result[i]);
	return result;
}

std::string String::trim(const std::string &str, const std::string &chars)
{
	size_t first = str.find_first_not_of(chars);
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(chars);
	return str.substr(first, last - first + 1);
}

std::string String::trim(const std::string &str)
{
	return (trim(str, " \t\n\r"));
}

std::vector<std::string> String::split(const std::string &str, const std::string &delim)
{
	std::vector<std::string> tokens;
	size_t start = str.find_first_not_of(delim);
	size_t end = str.find_first_of(delim, start);
	while (end != std::string::npos)
	{
		tokens.push_back(trim(str.substr(start, end - start)));
		start = str.find_first_not_of(delim, end);
		end = str.find_first_of(delim, start);
	}
	tokens.push_back(trim(str.substr(start)));
	return tokens;
}

bool String::isNumber(const std::string &str)
{
	if (str.empty())
		return false;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}

size_t String::toBytes(const std::string &str)
{
	size_t multiplier = 1;
	size_t pos = str.find_first_not_of("0123456789");
	size_t size = std::strtoul(str.c_str(), NULL, 10);
	if (pos == std::string::npos)
		return (size);
	std::string p = toLower(str.substr(pos));
	if (p == "k")
		multiplier = 1024;
	else if (p == "m")
		multiplier = 1024 * 1024;
	else if (p == "g")
		multiplier = 1024 * 1024 * 1024;
	else
		throw std::logic_error("Invalid size suffix");
	return (size * multiplier);
}