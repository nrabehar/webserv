#ifndef IO_IPP
#define IO_IPP 1

#include "../webserv.hpp"

class IOutput
{
	public: virtual ~IOutput() {};
		virtual void write(std::ostream &) const = 0;
};

class IInput
{
	public: virtual ~IInput() {};
		virtual void read(std::istream &) = 0;
};

#endif /* IO_IPP */
