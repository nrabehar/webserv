#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "../webserv.hpp"

class ConfigFile:
public virtual IConfigFile,
public virtual IOriginator<Config>
{

	private:

		std::string _name;

	public:

		ConfigFile();
		virtual ~ConfigFile();
		ConfigFile(const ConfigFile &);
		ConfigFile & operator=(const ConfigFile &);

		virtual const std::string & getName() const;
		virtual void setName(const std::string & name);

		virtual void load();

};

#endif
