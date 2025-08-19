#include "String.hpp"

std::string trimStr(const std::string &str)
{
	std::string spaces = " \t\n\r";
	size_t start = str.find_first_not_of(spaces);
	if (start == std::string::npos)
		return "";
	size_t end = str.find_last_not_of(spaces);
	return str.substr(start, end - start + 1);
}

std::vector<std::string> splitStr(const std::string &str, char delim)
{
	std::vector<std::string> res;
	std::stringstream sstr(str);
	std::string item;
	while (std::getline(sstr, item, delim))
	{
		item = trimStr(item);
		if (!item.empty())
			res.push_back(item);
	}
	return (res);
}

size_t toBytes(const std::string &entry)
{
	size_t multiplier = 1;
	size_t pos = entry.find_first_not_of("0123456789");
	size_t size = static_cast<size_t>(std::atoi(entry.c_str()));
	if (pos == std::string::npos)
		return (size);
	std::string p = entry.substr(pos);
	if (p == "K" || p == "k")
		multiplier = 1024;
	else if (p == "M" || p == "m")
		multiplier = 1024 * 1024;
	else if (p == "G" || p == "g")
		multiplier = 1024 * 1024 * 1024;
	else
		throw std::logic_error("Invalid size suffix");
	return (size * multiplier);
}
