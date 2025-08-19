#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <unistd.h>

class Client
{
	private:
		int _fd;

	public:
		Client(int fd);
		~Client();

		int getFd() const;

	private:
		Client(const Client &);
		Client &operator=(const Client &);
};

#endif
