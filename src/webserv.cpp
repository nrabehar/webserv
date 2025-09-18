#include "webserv.hpp"
#include <poll.h>

int main(int, char **)
{

	FileHandler	handler;
	FilePathBuilder path_builder(".", "");
	FileExistenceChecker f_exist;
	FilePermissionChecker p_check(R_OK);
	FileOpener f_open;
	
	handler.setNext(&path_builder)
				 ->setNext(&f_exist)
				 ->setNext(&p_check)
				 ->setNext(&f_open);

	IFile *file = handler.handle("Makefile");
	if (!file)
	std::cout << "Reason: " << handler.getReason() << std::endl;
	else
	{
		file->read();
		std::cout << "Readed file: " << file->getPath() << " on fd " << file->getFD() << std::endl;
	}
	

	IFile *f2 = handler.handle("Makefile");
	if (!f2)
		std::cout << "Reason: " << handler.getReason() << std::endl;
	else
		std::cout << "Readed file: " << f2->getPath() << " on fd " << f2->getFD() << std::endl;

	
	if (file)
		delete file;
	if (f2)
		delete f2;
	// delete handler;
	// delete path_builder;
	// delete f_exist;
	// delete p_check;
	// delete f_open;

	return (0);
}
