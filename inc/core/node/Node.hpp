#ifndef NODE_HPP
#define NODE_HPP

#include "../../webserv.hpp"

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
			_data.clear();
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

#endif // NODE_HPP