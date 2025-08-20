#ifndef UTILS_HPP
#define UTILS_HPP 1

#include "Logger.hpp"

#include <sstream>
#include <ctime>

class Utils
{
  public:
    static std::string & getDateTime();
    template <typename T> static std::string & toString(const T & value)
    {
      DBG(__METHOD_NAME__ + " called.");
      static std::string __str;
      std::ostringstream __oss;
      __oss << value;
      __str = __oss.str();
      DBG(__METHOD_NAME__ + " completed.");
      return (__str);
    }
    template <typename T> static T & fromString(const std::string & str)
    {
      static T __value;
      DBG(__METHOD_NAME__ + " called.");
      if (str.empty())
      {
        WRN("Utils::fromString() input string is empty.");
      }
      std::istringstream __iss(str);
      __iss >> __value;
      DBG(__METHOD_NAME__ + " completed.");
      return (__value);
    }
};

#define DATETIME() Utils::getDateTime()
#define TOSTRING(value) Utils::toString(value)
#define FROMSTRING(type, str) Utils::fromString<type>(str)

#endif // UTILS_HPP
