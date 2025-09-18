#include "webserv.hpp"

ConfigManager::~ConfigManager()
{
}

ConfigManager::ConfigManager(const std::string & f):
	_cfile(),
	_caretaker()
{
	this->cFile().setName(f);
	this->saveConfig();
}

void ConfigManager::saveConfig()
{
	this->cTaker().add(this->cFile().save());
}

void ConfigManager::restoreConfig()
{
	if (this->cTaker().size() > 0)
		this->cFile().restore(this->cTaker().get(this->cTaker().size() - 1));
}

void ConfigManager::restoreConfig(size_t idx)
{
	if (idx < this->cTaker().size())
		this->cFile().restore(this->cTaker().get(idx));
	else
		this->restoreConfig();
}

void ConfigManager::run()
{
	this->cFile().load();
}

void ConfigManager::reload()
{
	this->restoreConfig();
	this->_cfile.load();
}

Config & ConfigManager::config()
{
	return (this->_cfile.get());
}

ConfigFile & ConfigManager::cFile()
{
	return (this->_cfile);
}

ICaretaker<Config> & ConfigManager::cTaker()
{
	return (this->_caretaker);
}
