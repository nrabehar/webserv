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
    enum LogLevel { INFO, DEBUG, WARNING, ERROR };
    static void log(const std::string & msg, LogLevel level = INFO);
    static void debug(bool mode = false);
    static std::string & getMethodName(const std::string & pretty_function);
};

#define LOG(msg) Logger::log(msg, Logger::INFO)
#define DBG(msg) Logger::log(msg, Logger::DEBUG)
#define ERR(msg) Logger::log(msg, Logger::ERROR)
#define WRN(msg) Logger::log(msg, Logger::WARNING)
#define DEBUGMODE(mode) Logger::debug(mode)
#define __METHOD_NAME__ Logger::getMethodName(__PRETTY_FUNCTION__)

#include "Utils.hpp"

#endif // LOGGER_HPP
