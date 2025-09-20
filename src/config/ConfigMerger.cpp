#include "webserv.hpp"

ConfigMerger::ConfigMerger() {}
ConfigMerger::~ConfigMerger() {}

void	ConfigMerger::merge(Node<std::string> *parent, Node<std::string> *child)
{
	std::vector<Node<std::string> *> childs = parent->getChild();
	for (size_t i = 0; i < childs.size(); ++i)
	{
		Node<std::string> *directive = childs[i];
		if (directive->getChild().empty())
		{
			bool found = false;
			std::vector<Node<std::string> *> c_childs = child->getChild();
			for (size_t j = 0; j < c_childs.size(); ++j)
			{
				if (c_childs[i]->getName() == directive->getName())
					found = true;
			}
			if (!found)
				child->push(new Node<std::string>(*directive));
		}
	}
	for (size_t i = 0; i < child->getChild().size(); ++i)
		merge(child, child->getChild()[i]);
}
