#ifndef LOGGER_HPP
#define LOGGER_HPP 1

#include <iostream>

class Logger
{
  private:
    Logger();
    ~Logger();
    Logger(const Logger & src);
    Logger & operator=(const Logger & src);
  public:
    enum LogLevel { INFO, WARNING, DEBUG, ERROR };
    static void log(const std::string & msg, LogLevel level = INFO);
};

#define LOG(msg) Logger::log(msg, Logger::INFO)
#define ERR(msg) Logger::log(msg, Logger::ERROR)
#define WRN(msg) Logger::log(msg, Logger::WARNING)
#define DBG(msg) Logger::log(msg, Logger::DEBUG)

#include "Utils.hpp"

#endif // LOGGER_HPP
