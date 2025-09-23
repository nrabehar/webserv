#include "webserv.hpp"

Utils::~Utils() {}
Utils::Utils() {}
Utils::Utils(const Utils &) {}
Utils & Utils::operator=(const Utils &) { return (*this); }

bool Utils::braces(
	const std::string & str,
	const std::string & o,
	const std::string & c
)
{
	int nb = 0;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str.compare(i, o.length(), o) == 0) { nb++; i += o.length() - 1; }
		else if (str.compare(i, c.length(), c) == 0)
		{ nb--; i += c.length() - 1; if (nb < 0) { return (false); } }
	}
	return (nb == 0);
}

std::string Utils::sanitize(
	const std::string & str,
	const std::string & delim
)
{
	bool c = false;
	std::string result;
	std::string trimmed = Utils::trim(str, delim);

	for (size_t i = 0; i < trimmed.length(); i++)
	{
		if (delim.find(trimmed[i]) != std::string::npos)
		{ if (!c && !result.empty()) { result += " "; } c = true; }
		else { result += trimmed[i]; c = false; }
	}
	return (result);
}

std::string Utils::uncomment(
	const std::string & str,
	const std::string & symbol)
{
	std::string line = str;
	for (size_t start = line.find(symbol); start != std::string::npos;)
	{
		size_t end = line.find('\n', start);
		line.erase(start, end - start);
		start = line.find(symbol);
	}
	return (line);
}

std::string Utils::trim(
	const std::string & str,
	const std::string & delim
)
{
	size_t start = str.find_first_not_of(delim);
	if (start == std::string::npos) { return (""); }
	size_t end = str.find_last_not_of(delim);
	return (str.substr(start, end - start + 1));
}

std::vector<std::string> Utils::split(
	const std::string & str,
	const std::string & delim
)
{
	std::vector<std::string> token;
	size_t start = 0; size_t end = str.find_first_of(delim);
	for (; end != std::string::npos; )
	{
		if (end != start) { token.push_back(str.substr(start, end - start)); }
		start = end + 1; end = str.find_first_of(delim, start);
	}
	if (start < str.length()) { token.push_back(str.substr(start)); }
	return (token);
}

std::string Utils::join(
	const std::vector<std::string> & vec,
	const std::string & delim,
	const std::string & sep
)
{
	std::string str = "";
	for (size_t i = 0; i < vec.size(); i++)
	{
		str += Utils::trim(vec[i], delim);
		if (i + 1 < vec.size()) { str += sep; }
	}
	return (str);
}
