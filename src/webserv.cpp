#include "webserv.hpp"

int main(int, char **) {
	Ip _ip;
	std::cout << "Default: " << _ip << std::endl;
	std::cout << "Enter IP (host:port): ";
	std::cin >> _ip;
	std::cout << "IP: '" << _ip << "' ";
	if (_ip.valid())
	{
		std::cout << "is valid." << std::endl;
	}
	else
	{
		std::cout << "is invalid." << std::endl;
		return (1);
	}
	return (0);
}
