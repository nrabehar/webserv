#include "webserv.hpp"

int main(int, char **)
{

	IFile *local_file = FileFactory::create("webserv.conf");

	local_file->open();
	local_file->close();

	delete local_file;

	return (0);
}
