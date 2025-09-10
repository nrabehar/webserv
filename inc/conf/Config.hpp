#ifndef CONFIG_HPP
#define CONFIG_HPP 1

#include "../webserv.hpp"

class Config : public IValidator, public IParser
{
	private:
		ConfigFile _file;
	public:
		Config(const std::string &);
		~Config();
		EValidator validate() const;
		bool parse(const std::string & = "");
		ConfigFile & getFile();
	private:
		Config();
		Config(const Config &);
		Config & operator=(const Config &);
};

#endif // CONFIG_HPP
