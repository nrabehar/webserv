#ifndef UTILS_HPP
#define UTILS_HPP 1

#include "Logger.hpp"


class Utils
{
  private:
    Utils();
    ~Utils();
    Utils(const Utils & src);
    Utils & operator=(const Utils & src);
  public:
    template <typename T> static std::string & toString(const T & value)
    {
      DBG(METHOD_NAME + " called.");
      static std::string __str;
      std::ostringstream __oss;
      __oss << value;
      __str = __oss.str();
      DBG(METHOD_NAME + " completed.");
      return (__str);
    }
    template <typename T> static T & fromString(const std::string & str)
    {
      static T __value;
      DBG(METHOD_NAME + " called.");
      if (str.empty())
      {
        WRN(METHOD_NAME + " input string is empty.");
      }
      std::istringstream __iss(str);
      __iss >> __value;
      DBG(METHOD_NAME + " completed.");
      return (__value);
    }
};

#define TOSTRING(value) Utils::toString(value)
#define FROMSTRING(type, str) Utils::fromString<type>(str)

#endif // UTILS_HPP
