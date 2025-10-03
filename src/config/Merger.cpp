#include "webserv.hpp"

Config::Merger::Merger() {}
Config::Merger::~Merger() {}

void Config::Merger::merge(Node<Token> *parent, Node<Token> *child)
{
	if (!parent || !child) return;

	std::vector<Node<Token>*> og_child = child->getChild();
	const std::vector<Node<Token>*>& p_child = parent->getChild();

	for (size_t i = 0; i < p_child.size(); ++i)
	{
		Node<Token>* directive = p_child[i];

		const Directive* rule = DirectiveRegistry::get(directive->getName());
		if (!rule) continue;

		bool found = false;
		for (size_t j = 0; j < og_child.size(); ++j)
		{
			if (og_child[j] && og_child[j]->getName() == directive->getName())
			{
				found = true;
				break;
			}
		}
		if (found) continue;

		if (!rule->isAllowedIn(child->getName()))
			continue;

		child->addChild(directive->clone());

	}

	for (size_t k = 0; k < og_child.size(); ++k)
	{
		if (og_child[k])
			merge(child, og_child[k]);
	}
}

void Config::Merger::merge(Node<Token> *parent)
{
	if (!parent) return;

	std::vector<Node<Token>*>& p_child = parent->getChild();

	for (size_t i = 0; i < p_child.size(); ++i)
		merge(parent, p_child[i]);

}


void    Config::Merger::defLocation(Node<Token> *root)
{

	if (!root)
		return ;

	std::vector<Node<Token>* > servers;
	root->getChild(servers, "server");
	for (size_t i = 0; i < servers.size(); ++i)
	{
		std::vector<Node<Token>* > locs;
		servers[i]->getChild(locs, "location");
		bool has_default = false;
		for (size_t j = 0; j < locs.size(); ++j)
		{
			std::vector<Token> data = locs[j]->getData();
			if (data.size() > 0 && data[0].value == "/")
			{
				has_default = true;
				break;
			}
		}
		if (has_default)
			continue;
		Node<Token>* def = new Node<Token>("location");
		def->push(Token("/", 0));
		servers[i]->addChild(def);
	}

}
