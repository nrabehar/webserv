#ifndef CONFIG_HPP
#define CONFIG_HPP 1

#include "../webserv.hpp"

class Config
{

	private:

		ConfigFile _file;
		ConfigParser _parser;

	public:

		Config(const std::string &);
		~Config();
		ConfigFile & getFile();
		ConfigParser & getParser();

	private:

		Config();
		Config(const Config &);
		Config & operator=(const Config &);

};

#endif // CONFIG_HPP
