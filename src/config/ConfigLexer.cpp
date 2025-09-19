#include "webserv.hpp"

Token::Token(std::string n="", int l=0, int t=TK_STRING)
	: value(n), line(l), type(t) {}

ConfigLexer::ConfigLexer() {}
ConfigLexer::~ConfigLexer() {}

std::vector<Token> ConfigLexer::tokenize(const std::string &inp)
{
	std::vector<Token> tokens;
	size_t i = 0;
	int	line = 1;

	while (i < inp.size())
	{
		if (std::isspace(inp[i]))
		{
			if (inp[i] == '\n')
			{
				++line;
				if (!(tokens.back().type & TK_SYMBOL))
					tokens.push_back(Token(std::string(1, ';'), line - 1, TK_SYMBOL));
			}
			++i;
		}
		else if (isSymbol(inp[i]))
			tokens.push_back(Token(std::string(1, inp[i++]), line, TK_SYMBOL));
		else
		{
			size_t start = i;
			while (i < inp.size() && !std::isspace(inp[i]) && !isSymbol(inp[i]))
				++i;
			tokens.push_back(Token(inp.substr(start, i - start), line));
		}
	}
	tokens.push_back(Token("", line, TK_EOF));
	return (tokens);
}

bool	ConfigLexer::isSymbol(int c)
{
	std::string ss = "{};#";
	return (ss.find(c) != std::string::npos);
}
