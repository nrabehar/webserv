#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../webserv.hpp"

class Config
{

	private:

		IFile*						_file;
		std::string 			_path;
		Node<std::string>	*_root;

	public:

		Config(const std::string &);
		~Config();
		void	load();
		void	parse();

	private:

		Config();
		Config(const Config &);
		Config & operator=(const Config &);

};

#endif // CONFIG_HPP
