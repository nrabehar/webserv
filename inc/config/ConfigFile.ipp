#ifndef CONFIGFILE_IPP
#define CONFIGFILE_IPP

#include "../webserv.hpp"

class IConfigFile:
public virtual IFile
{

	public: virtual ~IConfigFile() {};

	virtual void load() = 0;

};

#endif
