#include "Logger.hpp"

#undef L
#define L "[ \e[32mLOG\e[0m ] "
#undef W
#define W "[ \e[33mWRN\e[0m ] "
#undef D
#define D "[ \e[34mDBG\e[0m ] "
#undef E
#define E "[ \e[31mERR\e[0m ] "

static bool _debug_mode = false;

void Logger::log(const std::string & msg, LogLevel level)
{
  switch (level)
  {
    case INFO:
      std::cout << "[ " << DATETIME() << " ] "<< L << msg << std::endl;
      break ;
    case WARNING:
      std::cout << "[ " << DATETIME() << " ] " << W << msg << std::endl;
      break ;
    case DEBUG:
      if (!_debug_mode)
        return ;
      std::cout << "[ " << DATETIME() << " ] " << D << msg << std::endl;
      break ;
    case ERROR:
      std::cerr << "[ " << DATETIME() << " ] " << E << msg << std::endl;
      break ;
  }
}

void Logger::debug(bool mode)
{
  _debug_mode = mode;
}


std::string & Logger::getMethodName(const std::string & pretty_function)
{
  static std::string __method_name;
  size_t __end;
  size_t __begin;
  size_t __delim;

  if (pretty_function.empty())
  {
    return (__method_name);
  }
  __delim = pretty_function.find("(");
  __begin = pretty_function.substr(0,__delim).rfind(" ") + 1;
  __end = pretty_function.rfind("(") - __begin;
  __method_name = pretty_function.substr(__begin,__end) + "()";
  return (__method_name);
}
