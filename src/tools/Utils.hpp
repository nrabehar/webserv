#ifndef UTILS_HPP
#define UTILS_HPP 1

#include <iostream>
#include <ctime>
#include <sstream>

#include "Logger.hpp"

class Utils
{
  public:
    static std::string & getDateTime();
    template <typename T> static std::string & toString(const T & value)
    {
      DBG("Utils::toString() called.");
      static std::string __str;
      std::ostringstream __oss;
      __oss << value;
      __str = __oss.str();
      DBG("Utils::toString() completed.");
      return (__str);
    }
    template <typename T> static T & fromString(const std::string & str)
    {
      static T __value;
      DBG("Utils::fromString() called.");
      if (str.empty())
      {
        WRN("Utils::fromString() input string is empty.");
      }
      std::istringstream __iss(str);
      __iss >> __value;
      DBG("Utils::fromString() completed.");
      return (__value);
    }
};

#define DATETIME() Utils::getDateTime()
#define TOSTRING(value) Utils::toString(value)
#define FROMSTRING(type, str) Utils::fromString<type>(str)

#endif // UTILS_HPP
