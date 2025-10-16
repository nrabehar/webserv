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

	return (s.substr(start, end - start + 1));

}

std::string String::trim(const std::string & s)
{

	const std::string delim = " \n\t\r\v\f";

	return (trim(s, delim));

}

std::vector<std::string> String::split(const std::string & s, const std::string & delim)
{

	std::vector<std::string> res;
	std::string	t_str;
	size_t	start = 0;
	size_t	end = 0;

	start = s.find_first_not_of(delim);
	while (start != std::string::npos)
	{

		end = s.find_first_of(delim, start);
		if (end == std::string::npos)
			end = s.size();
		
		res.push_back(s.substr(start, end - start));
		start = s.find_first_not_of(delim, end);

	}

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

std::string String::toLower(const std::string & s)
{

	std::string result = s;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return (result);

}

std::string String::toCamelCase(const std::string & s, char sep)
{

	std::string result;
	bool capitalize = false;

	for (size_t i = 0; i < s.length(); ++i)
	{
		if (s[i] == sep)
		{
			capitalize = true;
			result += sep;
		}
		else
		{
			if (capitalize || i == 0)
			{
				result += std::toupper(s[i]);
				capitalize = false;
			}
			else
			{
				result += std::tolower(s[i]);
			}
		}
	}

	return (result);

}

std::string String::urlEncode(const std::string & s)
{
	std::string result;
	
	for (size_t i = 0; i < s.length(); ++i)
	{
		char c = s[i];
		
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
		    (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~')
			result += c;
		else if (c == ' ')
			result += "%20";
		else
		{
			result += '%';
			result += "0123456789ABCDEF"[(c >> 4) & 0xF];
			result += "0123456789ABCDEF"[c & 0xF];
		}
	}
	
	return result;
}

std::string String::urlDecode(const std::string & s)
{

	std::string result;
	for (size_t i = 0; i < s.length(); ++i)
	{
		if (s[i] == '%' && i + 2 < s.length() && isxdigit(s[i + 1]) && isxdigit(s[i + 2]))
		{
			std::string hex = s.substr(i + 1, 2);
			char decoded_char = static_cast<char>(std::strtol(hex.c_str(), NULL, 16));
			result += decoded_char;
			i += 2;
		}
		else if (s[i] == '+')
		{
			result += ' ';
		}
		else
		{
			result += s[i];
		}
	}
	return (result);

}
