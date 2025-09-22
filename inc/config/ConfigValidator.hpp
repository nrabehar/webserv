#ifndef CONFIGVALIDATOR_HPP
#define CONFIGVALIDATOR_HPP

#include "../webserv.hpp"

class ConfigValidator
{

	private:
		
		static bool	_valid;
		static bool	_checked;
		static std::string		_status;

	public:
	
		ConfigValidator();
		virtual ~ConfigValidator();
		static bool	validate(Node<Token> * node);

	private:
		
		ConfigValidator(const ConfigValidator &);
		ConfigValidator & operator=(const ConfigValidator &);

		static void checkHttp(Node<Token> * node);
		static void checkServer(Node<Token> * node);
		static void checkLocation(Node<Token> * node);
		static void checkListen(Node<Token> * node);
		static void checkRoot(Node<Token> * node);
		static void checkIndex(Node<Token> * node);
		static void checkAutoindex(Node<Token> * node);
		static void checkErrorPage(Node<Token> * node);
		static void checkClientMaxBodySize(Node<Token> * node);
		static void checkKeepaliveTimeout(Node<Token> * node);
		static void checkServerName(Node<Token> * node);
		static void checkCgi(Node<Token> * node);
		static void checkAllowedMethod(Node<Token> * node);

};


#endif // CONFIGVALIDATOR_HPP