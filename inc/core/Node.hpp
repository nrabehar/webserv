#ifndef NODE_HPP
#define NODE_HPP

#include "../webserv.hpp"

template<typename T>
class Node
{

	protected:

		std::string								_name;
		std::vector<T>						_data;
		std::vector<Node<T> *> 		_child;
		Node<T> *									_parent;
		

	public:

		Node(const std::string & name)
			: _name(name), _parent(NULL) {};
		~Node()
		{
			for (size_t i = 0; i < _child.size(); ++i)
				delete _child[i];
			_child.clear();
		};

		void	push(const T & element) { _data.push_back(element); }
		void	addChild(Node<T> * node)
		{
			if (node)
			{
				node->_parent = this;
				_child.push_back(node);
			}
		}
		void	removeChild(Node<T> * node)
		{
			typename std::vector<Node<T>*>::iterator it;
			for (it = _child.begin(); it != _child.end() ; ++it)
			{
				if (*it == node)
				{
					delete *it;
					_child.erase(it);
					return ;
				}
			}
		}

		const std::string &	getName() const { return (_name); };
		const std::vector<T> &	getData() const { return (_data); };
		const std::vector<Node<T> *> & getChild() const { return (_child); };
		Node<T>*	getParent() const { return (_parent); };

	private:

		Node();
		Node(const Node &);
		Node & operator=(const Node<T> & );

};


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

	private:

		std::map<std::string, INodeValidator<T> * > _validator;

	public:

		ANodeValidator() {};

		~ANodeValidator()
		{
			typename std::map<std::string, INodeValidator<T> * >::iterator it;
			for (it = _validator.begin(); it != _validator.end(); ++it)
				delete it->second;
			_validator.clear();
		};

		void	with(const std::string & name, INodeValidator<T> * validator)
		{
			if (!validator)
				return ;
			typename std::map<std::string, INodeValidator<T> * >::iterator it;
			it = _validator.find(name);
			if (it != _validator.end())
			{
				delete it->second;
				it->second = validator;
			}
			else
				_validator[name] = validator;
		};

		virtual bool	validate(Node<T> *) = 0;

	private:

		ANodeValidator(const ANodeValidator &);
		ANodeValidator & operator=(const ANodeValidator &);

};


#endif // NODE_HPP