#ifndef CONFIG_IPP
#define CONFIG_IPP

#include "../webserv.hpp"

class IConfig
{

	public: virtual ~IConfig() {};

	virtual const std::string & getContent() const = 0;
	virtual void setContent(const std::string &) = 0;

};

#endif
