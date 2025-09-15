#ifndef IP_IPP
#define IP_IPP

#include "../webserv.hpp"

class IIp :
	public virtual IClear,
	public virtual ICopy,
	public virtual IClone,
	public virtual IIO,
	public virtual ICompare,
	public virtual IValid
{
	public:
		virtual ~IIp() {};
		virtual void setHost(const std::string &) = 0;
		virtual const std::string & getHost() const = 0;
		virtual void setPort(unsigned short) = 0;
		virtual unsigned short getPort() const = 0;
};

#endif // IP_IPP
