#ifndef CONFIGVALIDATOR_HPP
#define CONFIGVALIDATOR_HPP

#include "../webserv.hpp"

class IConfigValidator
{

	public:

		virtual ~IConfigValidator() {}
		virtual bool	validate(Node<Token>*) = 0;

};

class ConfigValidator: public IConfigValidator
{

	private:
		
		std::vector<IConfigValidator *> _validator;

	public:
	
		ConfigValidator();
		virtual ~ConfigValidator();
		virtual bool	validate(Node<Token>*);
		void	with(IConfigValidator *);

	private:
		
		ConfigValidator(const ConfigValidator &);
		ConfigValidator & operator=(const ConfigValidator &);

		bool	checkArgType(const std::string &, ArgType);

};


class CHttpValidator: public IConfigValidator
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