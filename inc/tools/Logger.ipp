#ifndef LOGGER_IPP
#define LOGGER_IPP

#include "../webserv.hpp"

class ILogger
{

	public: virtual ~ILogger() {};

	virtual void log(const std::string &, enum e_LogLevel) = 0;

};

#endif
