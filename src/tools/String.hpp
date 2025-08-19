#ifndef STRING_HPP
#define STRING_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>

std::string trimStr(const std::string &);

std::vector<std::string> splitStr(const std::string &, char delim);

template <typename T>
std::string toString(const T &val)
{
	std::ostringstream ostr;
	ostr << val;
	return (ostr.str());
}

size_t toBytes(const std::string &);

#endif
