#include "webserv.hpp"

std::map<std::string, Directive> DirectiveRegistry::_registry;
int DirectiveRegistry::_call = 0;

DirectiveRegistry::~DirectiveRegistry() {}

void DirectiveRegistry::save(const Directive & directive)
{
	_registry[directive.name()] = directive;
	_call++;
}

const Directive * DirectiveRegistry::get(const std::string &name)
{
	_call++;
	if (_registry.count(name))
	{
		const Directive * d = &_registry[name];
		return (d);
	}
	return (NULL);
}



