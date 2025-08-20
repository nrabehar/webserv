#include "Utils.hpp"

std::vector<std::string> Utils::extractBlocks(const std::string & content, const std::string & keyword)
{
  DBG(METHOD_NAME + " called.");
  std::vector<std::string> __blocks;
  size_t __start = 0;
  size_t __block_start = 0;
  size_t __bracket[2] = {0, 0};

  for (; __start < content.length();)
  {
    __block_start = content.find(keyword, __start);
    if (__block_start == std::string::npos)
    {
      break;
    }
    __bracket[0] = content.find('{', __block_start);
    if (__bracket[0] == std::string::npos)
    {
      break;
    }
    __bracket[1] = Utils::findMatchingBracket(content, __bracket[0]);
    if (__bracket[1] == std::string::npos)
    {
      break;
    }
    __blocks.push_back(content.substr(__block_start, __bracket[1] - __block_start + 1));
    __start = __bracket[1] + 1;
  }
  return (__blocks);
}

size_t Utils::findMatchingBracket(const std::string & content, size_t start)
{
  DBG(METHOD_NAME + " called.");
  int __count = 1;
  size_t __pos = start + 1;
  for (; __pos < content.length() && __count > 0; __pos++)
  {
    if (content[__pos] == '{')
    {
      __count++;
    }
    else if (content[__pos] == '}')
    {
      __count--;
    }
  }
  if (__count == 0)
  {
    return (__pos - 1);
  }
  WRN(METHOD_NAME + " no matching bracket found.");
  return (std::string::npos);
}
