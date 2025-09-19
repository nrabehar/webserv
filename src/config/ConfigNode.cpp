#include "webserv.hpp"

ConfigNode::ConfigNode(const std::string &name): _name(name) {}
ConfigNode::ConfigNode(const ConfigNode &other)
	: _name(other._name), _args(other._args) {
		for (size_t i = 0; i < other._child.size(); ++i)
		{
			_child.push_back(new ConfigNode(*other._child[i]));
		}
	}
ConfigNode::~ConfigNode()
{
	for (size_t i = 0; i < _child.size(); ++i)
		delete _child[i];
}

void ConfigNode::pushArg(const std::string &arg)
{
	_args.push_back(arg);
}

void ConfigNode::addChild(ConfigNode *child)
{
	if (child)
		_child.push_back(child);
}

const std::string&	ConfigNode::getName() const { return (_name); }
const std::vector<std::string>& ConfigNode::getArg() const { return (_args); }
std::vector<ConfigNode *> &ConfigNode::getChild() { return (_child); }

void ConfigNode::print(size_t indent = 0) const
{
	for (size_t i = 0; i < indent; ++i)
		std::cout << "	";
	std::cout << _name;
	for (size_t i = 0; i < _args.size(); ++i)
		std::cout << " " << _args[i];
	std::cout << std::endl;
	for (size_t i = 0; i < _child.size(); ++i)
		_child[i]->print(indent + 1);
}
