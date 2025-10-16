#ifndef DIRECTIVEPARSER_HPP
#define DIRECTIVEPARSER_HPP

#include "../../webserv.hpp"

template <typename T>
class IParser
{

	public:

		virtual ~IParser() {}
		virtual T * parse() = 0;

};


class DirectiveParser
	: virtual public IParser<Node<Token> >
{

	private:

		ITokenStream	& 		_stream;

	public:

		DirectiveParser(ITokenStream & stream);
		~DirectiveParser();
		Node<Token> *	parse();
	
	private:
	
		DirectiveParser();
		DirectiveParser(const DirectiveParser &);
		DirectiveParser & operator=(const DirectiveParser &);

		Node<Token>*	parseStatement();
		Node<Token>*	parseBlock(const std::string & name, const std::vector<Token> & arg);
		Node<Token>*	parseDirective(const std::string & name, const std::vector<Token> & arg);

		std::string line();

};



#endif
