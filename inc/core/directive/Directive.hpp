#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "../../webserv.hpp"


class Directive
{

	private:

		IFile *							_file;
		Node<Token> *				_data;
		bool 								_loaded;

	public:

		Directive(IFile * file);
		~Directive();

		void	load();
		const Node<Token> * get();


	private:

		Directive();
		Directive(const Directive &);
		Directive & operator=(const Directive &);

};


#endif // DIRECTIVE_HPP