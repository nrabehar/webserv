#ifndef DIRECTIVEREGISTRY_HPP
#define DIRECTIVEREGISTRY_HPP

#include "../../webserv.hpp"

class DirectiveRegistry
{

	private:

		static std::map<std::string, Directive> _registry;
		static int _call;

	public:

		~DirectiveRegistry();

		static void	save(const Directive &directive);
		static const Directive * get(const std::string &name);

	private:

		DirectiveRegistry();
		DirectiveRegistry(const DirectiveRegistry &);
		DirectiveRegistry & operator=(const DirectiveRegistry &);

};



#endif // DIRECTIVEREGISTRY_HPP