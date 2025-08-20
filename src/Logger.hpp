#ifndef LOGGER_HPP
#define LOGGER_HPP 1

#include <iostream>
#include <sstream>
#include <ctime>

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
    static void debugMode(bool mode = false);
    static std::string & getDateTime();
    static std::string & getMethodName(const std::string & pretty_function);
};

#define LOG(msg) Logger::log(msg, Logger::INFO)
#define DBG(msg) Logger::log(msg, Logger::DEBUG)
#define ERR(msg) Logger::log(msg, Logger::ERROR)
#define WRN(msg) Logger::log(msg, Logger::WARNING)
#define DATETIME() Logger::getDateTime()
#define DEBUGMODE(mode) Logger::debugMode(mode)
#define METHOD_NAME Logger::getMethodName(__PRETTY_FUNCTION__)

#include "Utils.hpp"

#endif // LOGGER_HPP
