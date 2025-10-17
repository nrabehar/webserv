#include "webserv.hpp"

Config::Sanitizer::Sanitizer() {}
Config::Sanitizer::~Sanitizer() {}

std::string	Config::Sanitizer::sanitize(const std::string & s)
{

	std::string	res;

	res = removeComments(s);

	res = normalizeNewlines(res);

	res = normalizeBrackets(res);

	return (res);

}

std::string Config::Sanitizer::removeComments(const std::string & s)
{

	std::string res;
	size_t pos = 0;

	while (pos < s.size())
	{

		size_t hash = s.find('#', pos);
		if (hash == std::string::npos)
		{
			res.append(s.substr(pos));
			break;
		}

		res.append(s.substr(pos, hash - pos));

		pos = s.find('\n', hash);
		if (pos == std::string::npos)
			break;

		res.push_back('\n');
		pos++;

	}

	return res;

}

std::string Config::Sanitizer::normalizeNewlines(const std::string & s)
{

	std::string res;

	for (size_t i = 0; i < s.size(); ++i)
	{

		if (s[i] == '\n')
		{

			if (res.empty() || res[res.size() - 1] != '\n')
				res.push_back('\n');

		}
		else
				res.push_back(s[i]);

	}

	if (!res.empty() && res[res.size() - 1] != '\n')
			res.push_back('\n');

	return (res);

}

std::string Config::Sanitizer::normalizeBrackets(const std::string & s)
{

	std::string res;

	for (size_t i = 0; i < s.size(); ++i)
	{

		int c = s[i];

		if (c == '{')
			res = String::trim(res) + " {";

		else
			res.push_back(c);

	}

	return (res);

}
