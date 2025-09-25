#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../webserv.hpp"
#include "Server.hpp"

//? Use this like namespace
class Config
{

	private:

		IFile*						_file;
		std::string 			_path;
		Node<Token>	*			_root;

	public:

		class Transformer;
		class Sanitizer;
		class Parser;
		class Merger;
		class Validator;


		Config(const std::string &);
		~Config();
		void	load();

		const std::vector<ServerConfig> &	servers() const;

	private:

		std::vector<ServerConfig> _servers;

		Config();
		Config(const Config &);
		Config & operator=(const Config &);

		void	parse();

};

#endif // CONFIG_HPP
