#ifndef CONFIGLEXER_HPP
#define CONFIGLEXER_HPP

#include "../webserv.hpp"

enum TokenType
{
	TK_NONE,
	TK_STRING  = 1 << 0,
	TK_SYMBOL  = 1 << 1,
	TK_EOL     = 1 << 2,
	TK_EOF     = 1 << 3
};

struct Token
{
	std::string	value;
	int					line;
	int				 	type;
	Token(std::string, int, int);
};

class ConfigLexer
{

	private:
		

	public:

		ConfigLexer();
		~ConfigLexer();
		
		static std::vector<Token> tokenize(const std::string &);

	private:

		ConfigLexer(const ConfigLexer &);
		ConfigLexer & operator=(const ConfigLexer &);

		static bool	isSymbol(int);

};

#endif // CONFIGLEXER_HPP