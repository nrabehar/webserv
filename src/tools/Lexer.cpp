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
		TokenType t = TokenU::inferType(std::string(1, inp[i]));
		if (std::isspace(inp[i]))
		{
			if (inp[i] == '\n')
			{
				++line;
				if (!(tokens.back().type & TK_SYMBOL))
				{
					std::cout << "Not end with symbol" << std::endl;
					tokens.push_back(Token(std::string(1, ';'), line - 1, TK_SYMBOL));
				}
			}
			++i;
			continue ;
		}
		else if (t & TK_SYMBOL)
			tokens.push_back(Token(std::string(1, inp[i++]), line, TK_SYMBOL));
		else
		{
			size_t start = i;
			while (i < inp.size() && !std::isspace(inp[i]) && !(t & TK_SYMBOL))
			{
				++i;
				t = TokenU::inferType(std::string(1, inp[i]));
			}
			tokens.push_back(Token(inp.substr(start, i - start), line));
		}
		std::cout << "Current token [" << tokens.back().value << "] with type "
							<< TokenU::typeToString(tokens.back().type) << std::endl;
	}
	tokens.push_back(Token("", line, TK_EOF));
	return (tokens);
}

