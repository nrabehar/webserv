#ifndef FILEHANDLER_IPP
#define FILEHANDLER_IPP

#include "../webserv.hpp"

class IFileHandler:
public virtual IGuest<IFileHandler, IFile>
{

	public: virtual ~IFileHandler() {};

};

#endif
