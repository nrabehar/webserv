#include "webserv.hpp"

ConfigMerger::ConfigMerger() {}
ConfigMerger::~ConfigMerger() {}

void	ConfigMerger::merge(ConfigNode *parent, ConfigNode *child)
{
	std::vector<ConfigNode *> childs = parent->getChild();
	for (size_t i = 0; i < childs.size(); ++i)
	{
		ConfigNode *directive = childs[i];
		if (directive->getChild().empty())
		{
			bool found = false;
			std::vector<ConfigNode *> c_childs = child->getChild();
			for (size_t j = 0; j < c_childs.size(); ++j)
			{
				if (c_childs[i]->getName() == directive->getName())
					found = true;
			}
			if (!found)
				child->addChild(new ConfigNode(*directive));
		}
	}
	for (size_t i = 0; i < child->getChild().size(); ++i)
		merge(child, child->getChild()[i]);
}
