#include "webserv.hpp"

int main(int, char **) {

	Config config("webserv.conf");
	std::cout << config.getFile().getContent() << std::endl;
	std::cout << config.getFile().getName() << std::endl;

	return (0);
}
