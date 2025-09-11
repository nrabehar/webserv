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

std::string String::trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t\n\r");
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, last - first + 1);
}

std::string String::trim(const std::string &str, const std::string &chars)
{
	size_t first = str.find_first_not_of(chars);
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(chars);
	return str.substr(first, last - first + 1);
}
