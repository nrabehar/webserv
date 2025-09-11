#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "../webserv.hpp"

class ConfigParser : public IParser, public IChecker, public IError
{

	private:

		const AFile & _file;
		std::vector<Server> _server;
		std::vector<std::string> _server_block;

	public:

		ConfigParser(const AFile &);
		~ConfigParser();

		const std::vector<Server> & getServer() const;
		const std::vector<std::string> & getServerBlock() const;

		void parse(const std::string &);
		EErrorCode check() const __attribute__((warn_unused_result));
		void reportError(EErrorCode);

		// @todo add necessary setters

		static CgiLink parseCgi(const std::string &);
		static AddrPort parseHostPort(const std::string &);
		static std::map<EStatusCode, std::string> parseErrorPage(const std::string &);
		static std::map<EStatusCode, std::string> parseRedirect(const std::string &);


	private:

		ConfigParser();
		ConfigParser(const ConfigParser &);
		ConfigParser & operator=(const ConfigParser &);

};

#endif // CONFIGPARSER_HPP