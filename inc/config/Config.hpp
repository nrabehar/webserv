#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../webserv.hpp"

class Config
{

	private:

		IFile*						_file;
		std::string 			_path;
		Node<Token>	*			_root;

	public:

		struct Server;
		class Transformer;


		Config(const std::string &);
		~Config();
		void	load();
		void	parse();

	private:

		std::vector<Server> _servers;

		Config();
		Config(const Config &);
		Config & operator=(const Config &);

};

#endif // CONFIG_HPP
