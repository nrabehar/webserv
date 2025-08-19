#include <iostream>
#include "./server/Webserv.hpp"
#include "./tools/Logger.hpp"
#include "./tools/String.hpp"

int main(int ac, char **av)
{
	if (ac == 2)
	{
		try
		{
			Signal::setup();
			WebServ server(av[1]);
			server.run();
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
			return 1;
		}
	}
	else
	{
		std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
		return 1;
	}
	return 0;
}
