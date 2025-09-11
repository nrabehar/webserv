#include "webserv.hpp"

int main(int, char **) {

	try
	{
		Config config("webserv.conf");
		std::string contents = config.getFile().getContent();
		config.getParser().parse(contents);
		const std::vector<Server> & servers = config.getServer();
		for (size_t i = 0; i < servers.size(); i++)
		{
			std::cout << "Server " << i + 1 << " has " << servers[i].getLocation().size() << " locations.\n";
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
