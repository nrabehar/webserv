#ifndef MANAGER_IPP
#define MANAGER_IPP

#include "../webserv.hpp"

class IManager
{

	public: virtual ~IManager() {};

	virtual void run() = 0;

};

#endif
