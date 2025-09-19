#include "webserv.hpp"

ConfigValidator::ConfigValidator() {}
ConfigValidator::~ConfigValidator() {
	for (size_t i = 0; i < _validator.size(); ++i)
		delete _validator[i];
}

bool	ConfigValidator::checkArgType(const std::string& arg, ArgType type)
{
	switch(type)
	{
		case NUMBER:
			return true;
		case STRING:
			return true;
		case ARRAY:
			return true;
		case BOOL:
			return arg=="on" || arg=="off";
		default: return false;
	}
}

void	ConfigValidator::validate(ConfigNode *node)
{
	// const DirectiveSpec *spec = _reg.get(node->getName());
	// if (!spec)
	// 	throw std::runtime_error("Unknown directive: " + node->getName());

	// if (std::find(spec->allowed_in.begin(), spec->allowed_in.end(), parent) == spec->allowed_in.end())
	// 	throw std::runtime_error("Directive " + node->getName() + " not allowed in " + parent);
	
	// if ((int)node->getArg().size() < spec->min_arg || (int)node->getArg().size() > spec->max_arg)	
	// 	throw std::runtime_error("Invalid number of args for "+node->getName());

	// for(size_t i=0;i<node->getArg().size();++i) {
	// 	ArgType type = (i<spec->arg_type.size())? spec->arg_type[i] : STRING;
	// 	if(!checkArgType(node->getArg()[i], type))
	// 			throw std::runtime_error("Argument type mismatch for "+node->getName());
	// }

	for (size_t i = 0; i < _validator.size(); ++i)
		_validator[i]->validate(node);

	for(size_t i=0;i<node->getChild().size();++i)
			validate(node->getChild()[i]);

}

void ConfigValidator::add(IConfigValidator *validator) { _validator.push_back(validator); }