#ifndef CONFIGDIRECTIVE_HPP
#define CONFIGDIRECTIVE_HPP

#include "webserv.hpp"

enum ArgType
{
	STRING,
	NUMBER,
	ARRAY,
	BOOL
};

struct DirectiveSpec
{
	std::string	name;
	bool	is_block;
	std::vector<std::string> allowed_in;
	int	min_arg;
	int	max_arg;
	std::vector<ArgType> arg_type;
	void clean();
};

class ConfigDirectiveRegistry
{

	private:
		
		std::vector<DirectiveSpec> _specs;

	public:

		ConfigDirectiveRegistry();
		~ConfigDirectiveRegistry();

		const DirectiveSpec* get(const std::string &) const;

	private:

		ConfigDirectiveRegistry(const ConfigDirectiveRegistry &);
		ConfigDirectiveRegistry & operator=(const ConfigDirectiveRegistry &);

};


#endif // CONFIGDIRECTIVE_HPP