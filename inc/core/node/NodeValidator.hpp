#ifndef NODEVALIDATOR_HPP
#define NODEVALIDATOR_HPP

#include "../../webserv.hpp"


template <typename T>
class INodeValidator
{

	public:

		virtual ~INodeValidator() {}
		virtual bool	validate(Node<T> * node) = 0;

};


template <typename T>
class ANodeValidator
	: virtual public INodeValidator<T>
{

	protected:

		std::map<std::string, INodeValidator<T> * > _validator;

	public:

		ANodeValidator() {};

		virtual ~ANodeValidator()
		{
			typename std::map<std::string, INodeValidator<T> * >::iterator it;
			for (it = _validator.begin(); it != _validator.end(); ++it)
				delete it->second;
			_validator.clear();
		};

		ANodeValidator*	with(const std::string & name, INodeValidator<T> * validator)
		{

			if (!validator)
				return (this);
			typename std::map<std::string, INodeValidator<T> * >::iterator it;
			it = _validator.find(name);
			
			if (it != _validator.end())
			{
				delete it->second;
				it->second = validator;
			}
			else
				_validator[name] = validator;
			return (this);

		};
		
		virtual bool validate(Node<T> * node)
		{
			size_t i = 0;
			for ( ; i < _validator.size(); ++i )
			{
				if (_validator[i]->validate())
					break ;
			}

			if (i >= _validator.size() && 
					(!node->getParent() && node->getNAme() == "base"))
				throw std::runtime_error("Unknown directive `" + node->getName() + "`" );
		
			for (i = 0; i < node->getChild().size() ; ++ i)
			{
				if (!validate(node->getChild()[i]))
					return (false);
			}

			return (true);
		
		}

	private:

		ANodeValidator(const ANodeValidator &);
		ANodeValidator & operator=(const ANodeValidator &);

};

template<typename T>
class NodeValidator
	: virtual public ANodeValidator<T>
{
	public:

		NodeValidator() {};
		virtual ~NodeValidator() {};
	
	private:

		NodeValidator(const NodeValidator &);
		NodeValidator & operator=(const NodeValidator &);

};


#endif