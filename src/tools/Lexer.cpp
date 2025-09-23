#include "webserv.hpp"

Lexer::Lexer() {}
Lexer::~Lexer() {}

std::vector<Token> Lexer::tokenize(const std::string &inp)
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
				if (tokens.size() && (!(tokens.back().type & (TK_SEMICOLON | TK_BRACE_C | TK_BRACE_O))))
					tokens.push_back(Token(std::string(1, ';'), line - 1, TK_SEMICOLON));
			}
			++i;
			continue ;
		}
		else if (TokenU::isSymbol(std::string(1, inp[i])))
		{
			tokens.push_back(Token(std::string(1, inp[i]), line));
			i++;
		}
		else
		{
			size_t start = i;
			while (i < inp.size() && !TokenU::isSymbol(std::string(1, inp[i])) &&
				!std::isspace(static_cast<unsigned char>(inp[i])))
				++i;
			tokens.push_back(Token(inp.substr(start, i - start), line));
		}
	}
	tokens.push_back(Token("", line, TK_EOF));
	return (tokens);
}

