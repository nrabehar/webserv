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

bool	ConfigValidator::validate(Node<std::string> *node)
{
	size_t i = 0;
	for (; i < _validator.size(); ++i)
	{
		if (_validator[i]->validate(node))
			break ;
	}

	if (i >= _validator.size() && !(!node->getParent() && node->getName() == "base"))
		throw std::runtime_error("Unknown directive " + node->getName());

	for(size_t i=0;i<node->getChild().size();++i)
			validate(node->getChild()[i]);
	return (true);
}

void ConfigValidator::add(IConfigValidator *validator) { _validator.push_back(validator); }


/**
 * @brief http directive validator
 */
CHttpValidator::CHttpValidator(){}
CHttpValidator::~CHttpValidator(){}
bool	CHttpValidator::validate(Node<std::string> *node)
{
	if (node->getName() != "http")
		return (false);
	if (node->getParent())
		throw std::runtime_error("Directive " + node->getName() + " not allowed in " + node->getParent()->getName());
	if (node->getData().size())
		throw std::runtime_error("Invalid number of args for " + node->getName());
	return (true);
}
