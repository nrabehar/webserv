#ifndef SERVER_HPP
#define SERVER_HPP 1

#include "../webserv.hpp"

struct ServerHostPort {
	std::string host;
	std::vector<int> port;
};

class Server: public IParser, public IChecker, public IError
{

	private:

		bool _autoindex;
		size_t _max_body_size;
		std::vector<Location> _location;
		std::vector<ServerHostPort> _hostport;
		std::map<EStatusCode, std::string> _errorpage;

	public:

		Server();
		~Server();
		Server(const Server &);
		Server & operator=(const Server &);

		bool getAutoIndex() const;
		size_t getMaxBodySize() const;
		const std::vector<Location> & getLocation() const;
		const std::vector<ServerHostPort> & getHostPort() const;
		const std::map<EStatusCode, std::string> & getErrorPage() const;

		void parse(const std::string &);
		EErrorCode check() const __attribute__((warn_unused_result));
		void reportError(EErrorCode);

		//@todo add necessary setters

};

#endif // SERVER_HPP
