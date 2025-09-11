#ifndef SERVER_HPP
#define SERVER_HPP 1

#include "../webserv.hpp"

struct AddrPort {
	int port;
	std::string addr;
	bool is_ipv6;
	AddrPort(): port(0), addr(""), is_ipv6(false) {}
};

class Server: public IParser, public IChecker, public IError
{

	private:

		bool _autoindex;
		std::string _root;
		size_t _max_body_size;
		std::vector<Location> _location;
		std::vector<AddrPort> _hostport;
		std::vector<std::string> _index;
		std::map<EStatusCode, std::string> _errorpage;
		std::map<EStatusCode, std::string> _redirect;

	public:

		Server();
		~Server();
		Server(const Server &);
		Server & operator=(const Server &);

		bool getAutoIndex() const;
		size_t getMaxBodySize() const;
		const std::string & getRoot() const;
		const std::vector<Location> & getLocation() const;
		const std::vector<AddrPort> & getHostPort() const;
		const std::vector<std::string> & getIndex() const;
		const std::map<EStatusCode, std::string> & getErrorPage() const;
		const std::map<EStatusCode, std::string> & getRedirect() const;

		void parse(const std::string &);
		EErrorCode check() const __attribute__((warn_unused_result));
		void reportError(EErrorCode);

		//@todo add necessary setters

	private:

		void setDirective(const std::string &, const std::string &);

};

#endif // SERVER_HPP
