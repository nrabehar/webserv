#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "webserv.hpp"

class ConfigParser
{

	private:
		
		std::vector<Token > _token;
		size_t	_pos;

	public:
	
		ConfigParser();
		~ConfigParser();
		Node<std::string>* parse(std::vector<Token> &);

	private:
		
		ConfigParser(const ConfigParser &);
		ConfigParser & operator=(const ConfigParser &);

		Node<std::string>*	parseStatement();
		Node<std::string>*	parseBlock(const std::string &, const std::vector<std::string> &);
		Node<std::string>*	parseDirective(const std::string &, const std::vector<std::string> &);
		
		Node<std::string>*	skipComment(const std::string &, const std::vector<std::string> &);

		bool	expectType(TokenType);
		std::string toStr(int);

};

#endif // CONFIGPARSER_HPP