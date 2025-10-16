#ifndef LEXER_HPP
#define LEXER_HPP

#include "../webserv.hpp"

class Lexer
{

	private:
		

	public:

		Lexer();
		~Lexer();
		
		static std::vector<Token> tokenize(const std::string &);

	private:

		Lexer(const Lexer &);
		Lexer & operator=(const Lexer &);

};

#endif