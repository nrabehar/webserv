#ifndef CONFIGNODE_HPP
#define CONFIGNODE_HPP

#include "../webserv.hpp"


template<typename T>
class Node
{

	protected:

		std::string								_name;
		std::vector<T>						_data;
		std::vector<Node<T> *> 		_child;
		const Node<T> *						_parent;
		

	public:

		Node(const std::string & name)
			: _name(name), _parent(NULL) {};
		Node(const Node<T> & other)
			:  _name(other._name), _data(other._data), _parent(other._parent)
		{
			for (size_t i = 0; i < other._child.size(); ++i)
				_child.push_back(other._child[i]);
		};
		~Node()
		{
			for (size_t i = 0; i < _child.size(); ++i)
				delete _child[i];
		};

		void	push(const T & element) { _data.push_back(element); }
		void	push(Node<T> * node)
		{
			if (node)
				_child.push_back(node);
		}

		const std::string &	getName() const { return (_name); };
		const std::vector<T> &	getData() const { return (_data); };
		std::vector<Node<T> *> & getChild() { return (_child); };
		const Node<T>*	getParent() const { return (_parent); };

	private:

		Node();
		Node & operator=(const Node<T> & );

};


#endif // CONFIGNODE_HPP