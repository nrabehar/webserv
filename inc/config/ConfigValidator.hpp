#ifndef CONFIGVALIDATOR_HPP
#define CONFIGVALIDATOR_HPP

#include "../webserv.hpp"

class ConfigValidator: public INodeValidator<Token>
{

	private:
		
		std::vector<INodeValidator *> _validator;

	public:
	
		ConfigValidator();
		virtual ~ConfigValidator();
		virtual bool	validate(Node<Token>*);
		ConfigValidator *	with(INodeValidator *);

	private:
		
		ConfigValidator(const ConfigValidator &);
		ConfigValidator & operator=(const ConfigValidator &);

};


class CHttpValidator
	: public INodeValidator<Token>
{

	public:

		CHttpValidator();
		virtual ~CHttpValidator();
		virtual bool	validate(Node<Token>*);
	
	private:

		CHttpValidator(const CHttpValidator &);
		CHttpValidator&	operator=(const CHttpValidator &);

};

#endif // CONFIGVALIDATOR_HPP