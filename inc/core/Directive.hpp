#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "../webserv.hpp"


class Directive
{

	private:

		IFile *							_file;
		Node<std::string> *	_data;

	public:

		Directive(IFile * file);
		~Directive();

		void	load();
		const Node<std::string> * get() const;


	private:

		Directive();
		Directive(const Directive &);
		Directive & operator=(const Directive &);

};


#endif // DIRECTIVE_HPP