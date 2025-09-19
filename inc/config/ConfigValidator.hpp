#ifndef CONFIGVALIDATOR_HPP
#define CONFIGVALIDATOR_HPP

#include "../webserv.hpp"

class ConfigValidator
{

	private:
		
		const ConfigDirectiveRegistry& _reg;

	public:
	
		ConfigValidator(const ConfigDirectiveRegistry &);
		~ConfigValidator();
		void	validate(ConfigNode*, const std::string &);

	private:
		
		ConfigValidator();
		ConfigValidator(const ConfigValidator &);
		ConfigValidator & operator=(const ConfigValidator &);

		bool	checkArgType(const std::string &, ArgType);

};

#endif // CONFIGVALIDATOR_HPP