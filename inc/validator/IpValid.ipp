#ifndef IPVALID_IPP
#define IPVALID_IPP

#include "../webserv.hpp"

class IIpValid :
	public virtual IValid,
	public virtual IGuest
{
	public:
		virtual ~IIpValid() {};
};

#endif
