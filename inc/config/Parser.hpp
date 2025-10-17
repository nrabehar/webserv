#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "../webserv.hpp"

class Config::Parser
{

	private:

		static std::vector<Token> _token;
		static size_t	_pos;

	public:

		Parser();
		~Parser();
		static Node<std::string>* parse(std::vector<Token> & token);

	private:

		Parser(const Parser &);
		Parser & operator=(const Parser &);

		static Node<std::string>*	parseStatement();
		static Node<std::string>*	parseBlock(const std::string &, const std::vector<std::string> &);
		static Node<std::string>*	parseDirective(const std::string &, const std::vector<std::string> &);
		static Node<std::string>*	skipComment(const std::string &, const std::vector<std::string> &);

		static bool	expectType(TokenType);
		static std::string toStr(int);

};

#endif
