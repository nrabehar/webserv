#ifndef CFGVALID_IPP
#define CFGVALID_IPP

#include "../webserv.hpp"

class ICfgValid :
	public virtual IValid,
	public virtual IGuest
{
	public:
		virtual ~ICfgValid() {};
};

#endif
