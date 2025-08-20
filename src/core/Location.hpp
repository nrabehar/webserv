#ifndef LOCATION_HPP
#define LOCATION_HPP 1

#include <map>
#include <vector>
#include "Logger.hpp"

class Location
{
  private:
    std::map<std::string, std::vector<std::string> > _data;
  public:
    Location();
    ~Location();
    const std::vector<std::string> & get(const std::string &) const;
    void set(const std::string &, const std::vector<std::string> &);
  private:
    Location(const Location &);
    Location & operator=(const Location &);
};

#endif // LOCATION_HPP
