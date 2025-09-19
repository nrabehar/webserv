#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "webserv.hpp"

class ConfigParser
{

	private:
		
		static std::vector<Token > _token;
		static size_t	_pos;

	public:
	
		~ConfigParser();
		static ConfigNode* parse(std::vector<Token> &);

	private:
		
		ConfigParser();
		ConfigParser(const ConfigParser &);
		ConfigParser & operator=(const ConfigParser &);

		static ConfigNode*	parseStatement();
		static ConfigNode*	parseBlock(const std::string &, const std::vector<std::string> &);
		static ConfigNode*	parseDirective(const std::string &, const std::vector<std::string> &);
		
		static ConfigNode*	skipComment(const std::string &, const std::vector<std::string> &);

		static bool	expectType(TokenType);
		static std::string toStr(int);

};

#endif // CONFIGPARSER_HPP