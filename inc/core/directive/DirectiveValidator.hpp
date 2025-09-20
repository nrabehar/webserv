#ifndef DIRECTIVEVALIDATOR_HPP
#define DIRECTIVEVALIDATOR_HPP

#include "../../webserv.hpp"

class DirectiveValidator
	: virtual public INodeValidator<std::string>
{

	private:

		

	public:

		DirectiveValidator();
		~DirectiveValidator();
		bool	validate(Node<std::string> * node);

	private:

		DirectiveValidator(const DirectiveValidator &);
		DirectiveValidator & operator=(const DirectiveValidator &);

};



#endif // DIRECTIVEVALIDATOR_HPP