#include "Logger.hpp"

#undef L
#define L ":[\e[32mLOG\e[0m] "
#undef W
#define W ":[\e[33mWRN\e[0m] "
#undef D
#define D ":[\e[34mDBG\e[0m] "
#undef E
#define E ":[\e[31mERR\e[0m] "

void Logger::log(const std::string & msg, LogLevel level)
{
  switch (level)
  {
    case INFO:
      std::cout << DATETIME() << L << msg << std::endl;
      break;
    case WARNING:
      std::cout << DATETIME() << W << msg << std::endl;
      break;
    case DEBUG:
      std::cout << DATETIME() << D << msg << std::endl;
      break;
    case ERROR:
      std::cerr << DATETIME() << E << msg << std::endl;
      break;
  }
}
