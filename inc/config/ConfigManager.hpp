#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include "../webserv.hpp"

class ConfigManager:
public virtual IConfigManager
{

	private:

		ConfigFile					_cfile;
		ICaretaker<Config>	_caretaker;

	public:

		virtual ~ConfigManager();
		ConfigManager(const std::string &);

		virtual void saveConfig();
		virtual void restoreConfig();
		virtual void restoreConfig(size_t);

		virtual void run();

		Config & config();
		ConfigFile & cFile();
		ICaretaker<Config> & cTaker();

		virtual void reload();

	private:

		ConfigManager();
		ConfigManager(const ConfigManager &);
		ConfigManager & operator=(const ConfigManager &);

};

#endif
