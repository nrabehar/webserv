#ifndef CONFIGMANAGER_IPP
#define CONFIGMANAGER_IPP

#include "../webserv.hpp"

class IConfigManager:
public virtual IManager
{

	public: virtual ~IConfigManager() {};

	virtual void saveConfig() = 0;
	virtual void restoreConfig() = 0;
	virtual void restoreConfig(size_t) = 0;

};

#endif
