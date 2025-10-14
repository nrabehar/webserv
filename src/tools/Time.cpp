#include "webserv.hpp"

Time::Time() {}
Time::~Time() {}

std::time_t	Time::now() { return std::time(NULL); }
std::string	Time::timeToStr(std::time_t t, const std::string & format)
{
	char	buf[256];
	struct std::tm	*tm_info = std::localtime(&t);
	if (tm_info == NULL)
		throw std::runtime_error("Could not convert time to localtime");
	if (std::strftime(buf, sizeof(buf), format.c_str(), tm_info) == 0)
		throw std::runtime_error("Could not format time to string");
	return std::string(buf);
}
long		Time::diff(std::time_t start, std::time_t end) { return static_cast<long>(end - start); }
std::time_t	Time::toTime(const std::string &str)
{
	struct std::tm	tm;
	if (str.empty())
		throw std::invalid_argument("Empty time string");
	if (strptime(str.c_str(), "%a %b %d %H:%M:%S %Y", &tm) == NULL)
		throw std::invalid_argument("Invalid time string format: " + str);
	return std::mktime(&tm);
}

std::time_t	Time::toTime(int t) { return static_cast<std::time_t>(t); }
std::string	Time::timeToHttpStr(std::time_t t) { return timeToStr(t, "%a, %d %b %Y %H:%M:%S GMT"); }
