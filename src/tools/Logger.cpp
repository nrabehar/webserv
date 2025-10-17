#include "webserv.hpp"

Logger::Logger()
{
}

Logger::~Logger()
{
}

Logger::Logger(const Logger &)
{
}

Logger & Logger::operator=(const Logger &)
{
	return (*this);
}

void Logger::log(const std::string & m, e_LogLevel l)
{
	std::string lvl = "INFO";
	std::ostream * os = &std::cout;
	switch (l)
	{
		case LOG_DEBUG: lvl = "DEBUG"; break;
		case LOG_INFO: lvl = "INFO"; break;
		case LOG_WARNING: lvl = "WARNING"; break;
		case LOG_ERROR: os = &std::cerr; lvl = "ERROR"; break;
		default: break;
	}
	(*os) << "[ " << Time::timeToStr(Time::now()) << " ] [ " << lvl << " ]: " << m << std::endl;
}
