#include "webserv.hpp"

int main(int, char **) {

	try
	{
		Config config("webserv.conf");
		(void)config;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
