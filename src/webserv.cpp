#include "webserv.hpp"
#include <poll.h>

int main(int, char **)
{

	IFile * f1 = new FileProxy(new File("Makefile"));
	IFile * f2 = new FileProxy(new File("src/webserv.cpp")); // not used, just to test
	IFile * f3 = new FileProxy(new File("Makefile")); // duplicate to test cache

	std::vector<IFile *> files;
	files.push_back(f1);
	files.push_back(f2);
	files.push_back(f3);

	while (!files.empty())
	{
		std::vector<struct pollfd> pfds;
		for (size_t i = 0; i < files.size(); ++i) {
			struct pollfd pfd;
			pfd.fd = files[i]->getFD();
			pfd.events = POLLIN;
			pfd.revents = 0;
			pfds.push_back(pfd);
		}

		int ret = ::poll(&pfds[0], pfds.size(), -1);
		if (ret == -1)
		{
			std::cerr << "Error in poll(): " << std::strerror(errno) << std::endl;
			return (1);
		}
		for (size_t i = 0; i < pfds.size(); ++i) {
			if (pfds[i].revents & POLLIN) {
				time_t now = time(NULL);
				files[i]->read();
			}
		}
		for (size_t i = 0; i < files.size(); ++i) {
			if (files[i]->isComplete()) {
				std::cout << "File " << files[i]->getPath()
							<< " on fd " << files[i]->getFD()
							<< " complete, size: " << files[i]->getData().size() << std::endl;
				files.erase(files.begin() + i);
			}
		}
	}

	delete f1;
	delete f2;
	delete f3;

	return (0);
}
