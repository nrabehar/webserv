#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../webserv.hpp"

class Config:
public virtual IConfig
{

	private:

		IFile*			_file;
		std::string _path;
		ConfigNode	*_root;

	public:

		Config(const std::string &);
		~Config();
		void	load();
		void	parse();

	private:

		Config();
		Config(const Config &);
		Config & operator=(const Config &);

		const std::string & getContent() const;
		void setContent(const std::string &);
};

#endif // CONFIG_HPP
