#ifndef FILE_IPP
#define FILE_IPP

#include "../webserv.hpp"

class IFile:
public virtual IHost<IFile, IFileHandler>
{

	public: virtual ~IFile() {};

	virtual const std::string & getName() const = 0;
	virtual void setName(const std::string &) = 0;

};

#endif
