#ifndef SIGNAL_IPP
#define SIGNAL_IPP

#include "../webserv.hpp"

class ISignal
{

	public: virtual ~ISignal() {};

	virtual void setup() = 0;

};

#endif
