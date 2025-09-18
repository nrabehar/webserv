#include "webserv.hpp"

ConfigFile::ConfigFile():
IOriginator<Config>(Config()),
_name(DEFAULT_CONFIG_FILE_NAME)
{
}

ConfigFile::~ConfigFile()
{
}

ConfigFile::ConfigFile(const ConfigFile & src):
IOriginator<Config>(Config()),
_name(DEFAULT_CONFIG_FILE_NAME)
{
	(*this) = src;
}

ConfigFile & ConfigFile::operator=(const ConfigFile & src)
{
	if (this == &src)
		return (*this);
	this->setName(src.getName());
	this->setState(src.getState());
	return (*this);
}

ConfigFile & ConfigFile::operator=(const IConfigFile & src)
{
	if (this == &src)
		return (*this);
	this->setName(src.getName());
	this->setState(src.getState());
	return (*this);
}

const std::string & ConfigFile::getName() const
{
	return (this->_name);
}

void ConfigFile::setName(const std::string & name)
{
	this->_name = name;
}

void ConfigFile::load()
{
}
