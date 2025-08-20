#ifndef UTILS_HPP
#define UTILS_HPP 1

#include "Logger.hpp"
#include <vector>

class Utils
{
  private:
    Utils();
    ~Utils();
    Utils(const Utils &);
    Utils & operator=(const Utils &);
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
    static std::vector<std::string> extractBlocks(const std::string &, const std::string &);
    static size_t findMatchingBracket(const std::string &, size_t);
};

#define TOSTRING(value) Utils::toString(value)
#define FROMSTRING(type, str) Utils::fromString<type>(str)

#endif // UTILS_HPP
