#ifndef CONFIGVALIDATOR_HPP
#define CONFIGVALIDATOR_HPP

#include "../webserv.hpp"

class Config::Validator
{

	private:

		static bool	_valid;
		static bool	_checked;
		static std::string		_status;

	public:

		Validator();
		virtual ~Validator();
		static bool	validate(Node<Token> * node);

	private:

		Validator(const Validator &);
		Validator & operator=(const Validator &);

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
		static void checkGatewayTimeout(Node<Token> * node);
		static void checkServerName(Node<Token> * node);
		static void checkCgi(Node<Token> * node);
		static void checkAllowedMethod(Node<Token> * node);
		static void checkReturn(Node<Token> * node);
		static void checkUploadStore(Node<Token> * node);
};

#endif
