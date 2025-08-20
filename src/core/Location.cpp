#include "Location.hpp"

Location::~Location()
{
  DBG(METHOD_NAME + " called.");
}

Location::Location()
{
  DBG(METHOD_NAME + " called.");
}

const std::vector<std::string> & Location::get(const std::string & key) const
{
  static const std::vector<std::string> __empty_vector;
  DBG(METHOD_NAME + " called.");
  if (key.empty())
  {
    WRN("Key is empty.");
    return (__empty_vector);
  }
  std::map<std::string, std::vector<std::string> >::const_iterator it;
  it = _data.find(key);
  if (it != _data.end())
  {
    return (it->second);
  }
  WRN("Key not found: " + key);
  return (__empty_vector);
}

void Location::set(const std::string & k, const std::vector<std::string> & v)
{
  DBG(METHOD_NAME + " called.");
  if (k.empty())
  {
    WRN("Key is empty, cannot set value.");
    return ;
  }
  _data.insert(std::make_pair(k, v));
}
