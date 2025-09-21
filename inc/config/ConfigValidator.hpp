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
		void	with(INodeValidator *);

	private:
		
		ConfigValidator(const ConfigValidator &);
		ConfigValidator & operator=(const ConfigValidator &);

		bool	checkArgType(const std::string &, ArgType);

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