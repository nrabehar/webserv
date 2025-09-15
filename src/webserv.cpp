#include "webserv.hpp"

int main(int, char **) {
	Ip _ip;
	std::string ip = "192.168.1.1:8080";
	std::cout << _ip << std::endl;
	std::istringstream iss(ip);
	iss >> _ip;
	std::cout << _ip << std::endl;
	return (0);
}
