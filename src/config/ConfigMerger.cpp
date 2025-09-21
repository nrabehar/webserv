#include "webserv.hpp"

ConfigMerger::ConfigMerger() {}
ConfigMerger::~ConfigMerger() {}

void	ConfigMerger::merge(Node<Token> *parent, Node<Token> *child)
{
	std::vector<Node<Token> *> childs = parent->getChild();
	for (size_t i = 0; i < childs.size(); ++i)
	{
		Node<Token> *directive = childs[i];
		if (directive->getChild().empty())
		{
			bool found = false;
			std::vector<Node<Token> *> c_childs = child->getChild();
			for (size_t j = 0; j < c_childs.size(); ++j)
			{
				if (c_childs[i]->getName() == directive->getName())
					found = true;
			}
			if (!found)
				child->addChild(directive);
		}
	}
	for (size_t i = 0; i < child->getChild().size(); ++i)
		merge(child, child->getChild()[i]);
}
