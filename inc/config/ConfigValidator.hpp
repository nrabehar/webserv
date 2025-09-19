#ifndef CONFIGVALIDATOR_HPP
#define CONFIGVALIDATOR_HPP

#include "../webserv.hpp"

class IConfigValidator
{

	public:

		virtual ~IConfigValidator() {}
		virtual void	validate(ConfigNode*) = 0;

};

class ConfigValidator: public IConfigValidator
{

	private:
		
		std::vector<IConfigValidator *> _validator;

	public:
	
		ConfigValidator();
		virtual ~ConfigValidator();
		virtual void	validate(ConfigNode*);
		void	add(IConfigValidator *);

	private:
		
		ConfigValidator(const ConfigValidator &);
		ConfigValidator & operator=(const ConfigValidator &);

		bool	checkArgType(const std::string &, ArgType);

};

#endif // CONFIGVALIDATOR_HPP