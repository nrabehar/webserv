#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "../../webserv.hpp"

#define DEFAULT_DIRECTIVE_PATH "conf/conf.directive"

class Directive
{

	private:

		std::string					_path;
		IFile *							_file;
		Node<Token> *				_data;
		bool 								_loaded;

	public:

		Directive();
		Directive(const std::string & path);
		~Directive();

		void	load();
		const Node<Token> * get();


	private:

		Directive(const Directive &);
		Directive & operator=(const Directive &);

};


#endif // DIRECTIVE_HPP