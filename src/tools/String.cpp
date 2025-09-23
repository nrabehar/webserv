#include "webserv.hpp"

String::~String() {}

std::string String::trim(const std::string & s, const std::string & delim)
{

	std::string res;

	size_t start;
	size_t end;

	start = s.find_first_not_of(delim);
	end = s.find_last_not_of(delim);

	if (start == std::string::npos)
		return (s);

	return (s.substr(start, end));

}

std::string String::trim(const std::string & s)
{

	const std::string delim = " \n\t";

	return (trim(s, delim));

}

std::vector<std::string> String::split(const std::string & s, const std::string & delim)
{

	std::vector<std::string> res;
	std::string	t_str;
	size_t	start = 0;
	size_t	end = 0;

	t_str = trim(s, delim);
	end = s.find_first_of(delim);

	while (end != std::string::npos && start < s.size())
	{

		if (end > start)
			res.push_back(s.substr(start, end - start));
		
		start = s.find_first_not_of(delim, end);
		
		if (start != std::string::npos)
			end = s.find_first_of(delim, start);

	}

	if (start < s.size())
		res.push_back(s.substr(start));

	return (res);

}

std::vector<std::string> String::split(const std::string & s)
{

	const std::string delim = " \n\t";

	return (split(s, delim));

}

bool String::isNumeric(const std::string & s)
{

	const std::string	n = "0123456789";

	return (s.find_first_not_of(n) == std::string::npos);

}
