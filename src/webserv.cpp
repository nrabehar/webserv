#include "webserv.hpp"
#include <poll.h>

int main(int, char **)
{

	try
	{
		Config conf("webserv.conf");
		conf.load();
		conf.parse();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}

	return (0);
}
