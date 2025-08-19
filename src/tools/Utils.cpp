#include "Utils.hpp"

std::string & Utils::getDateTime()
{
  std::tm *__tm;
  std::time_t __now;
  char __buffer[24];
  static std::string __current_time;

  __now = std::time(NULL);
  __tm = std::localtime(&__now);
  std::strftime(__buffer, sizeof(__buffer), "%Y-%m-%d..%I:%M:%S.%p", __tm);
  __current_time = __buffer;
  return (__current_time);
}
