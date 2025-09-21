#include "webserv.hpp"

ConfigValidator::ConfigValidator() {}
ConfigValidator::~ConfigValidator() {
	for (size_t i = 0; i < _validator.size(); ++i)
		delete _validator[i];
}

bool	ConfigValidator::validate(Node<Token> *node)
{
	std::cout << "Trying to validate " << node->getName()
						<< " with " << node->getChild().size() << " childrens" << std::endl;
	size_t i = 0;
	for (; i < _validator.size(); ++i)
	{
		if (_validator[i]->validate(node))
			break ;
	}

	if (i >= _validator.size() && !(!node->getParent() && node->getName() == "base"))
		throw std::runtime_error("Unknown directive `" + node->getName() + "`");

	for(size_t i=0;i<node->getChild().size();++i)
			validate(node->getChild()[i]);
	return (true);
}

ConfigValidator * ConfigValidator::with(INodeValidator *validator)
{

	if (validator)
		_validator.push_back(validator);

	return (this);

}


/**
 * @brief http directive validator
 */
CHttpValidator::CHttpValidator(){}
CHttpValidator::~CHttpValidator(){}
bool	CHttpValidator::validate(Node<Token> *node)
{

	if (node->getName() != "http")
		return (false);
	return NodeChecker::check(node)
								.requireChild()
								.acceptParent("base")
								.argCount(0, 0).isValid();
}
