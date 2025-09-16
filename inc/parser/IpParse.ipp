#ifndef IPPARSE_IPP
#define IPPARSE_IPP

#include "../webserv.hpp"

class IIpParse :
	public virtual IParse,
	public virtual IGuest
{
	public:
		virtual ~IIpParse() {};
};

#endif
