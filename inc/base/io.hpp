#ifndef IO_HPP
#define IO_HPP 1

#include "../webserv.hpp"

std::ostream & operator<<(std::ostream &, const IOutput &);
std::istream & operator>>(std::istream &, IInput &);

#endif /* IO_HPP */
