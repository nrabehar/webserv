#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "../webserv.hpp"

class Logger:
public virtual ILogger
{

	public:

		Logger();
		virtual ~Logger();
		Logger(const Logger &);
		Logger & operator=(const Logger &);

		void log(const std::string &, enum e_LogLevel);

};

#undef LOG
#define LOG(msg) Logger().log(msg, LOG_INFO)

#undef DBG
#define DBG(msg) Logger().log(msg, LOG_DEBUG)

#undef WRN
#define WRN(msg) Logger().log(msg, LOG_WARNING)

#undef ERR
#define ERR(msg) Logger().log(msg, LOG_ERROR)

#endif
