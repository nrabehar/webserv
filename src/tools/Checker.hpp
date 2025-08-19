#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "String.hpp"

bool isNumeric(const std::string &);
bool isValidMethod(const std::string &);
bool isValidPort(size_t);
bool isValidIP(const std::string &);

#endif