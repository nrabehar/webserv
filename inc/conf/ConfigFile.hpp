#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP 1

#include "../webserv.hpp"

class ConfigFile : public AFile
{
	public:
		~ConfigFile();
		ConfigFile(const std::string &);
	private:
		ConfigFile();
		ConfigFile(const ConfigFile &);
		ConfigFile & operator=(const ConfigFile &);
};

#endif // CONFIGFILE_HPP
