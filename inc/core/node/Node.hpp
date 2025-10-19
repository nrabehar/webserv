#ifndef NODE_HPP
#define NODE_HPP

#include "../../webserv.hpp"
#include "../memory/memory.hpp"

template<typename T>
class Node
{

  protected:

    std::string               _name;
    std::vector<T>            _data;
    std::vector<Node<T> *>    _child;
    Node<T> *                 _parent;

  public:

    Node(const std::string & name)
      : _name(name), _parent(NULL) {};
    ~Node()
    {
      for (size_t i = 0; i < _child.size(); ++i)
        ft::free(_child[i]);
      _child.clear();
      _data.clear();
    };

    void  push(const T & element) { _data.push_back(element); }
    void  addChild(Node<T> * node)
    {
      if (node)
      {
        node->_parent = this;
        _child.push_back(node);
      }
    }
    void  removeChild(Node<T> * node)
    {
      typename std::vector<Node<T>*>::iterator it;
      for (it = _child.begin(); it != _child.end() ; ++it)
      {
        if (*it == node)
        {
          ft::free(*it);
          _child.erase(it);
          return ;
        }
      }
    }

    Node<T>* clone()
    {
        Node<T>* dst = ft::alloc<Node<T> >(_name);
        for (size_t i = 0; i < _data.size(); ++i)
            dst->push(_data[i]);

        for (size_t i = 0; i < _child.size(); ++i)
            dst->addChild(_child[i]->clone());

        return (dst);
    }

    void print(int depth = 0)
    {
      for (int i = 0; i < depth; ++i)
        std::cout << "  ";

      std::cout << _name;

      if (!_data.empty())
      {
        std::cout << " (";
        for (size_t i = 0; i < _data.size(); ++i)
        {
          std::cout << _data[i].value;
          if (i + 1 < _data.size())
              std::cout << ", ";
        }
        std::cout << ")";
      }

      std::cout << std::endl;
      for (size_t i = 0; i < _child.size(); ++i)
        _child[i]->print(depth + 1);
    }

    const std::string & getName() const { return (_name); };
    const std::vector<T> &  getData() const { return (_data); };
    std::vector<Node<T> *> & getChild() { return (_child); };
    void getChild(std::vector<Node<T> * > &out, const std::string & name)
    {
      for (size_t i = 0; i < _child.size(); ++i)
      {
        if (_child[i] && _child[i]->_name == name)
          out.push_back(_child[i]);
        _child[i]->getChild(out, name);
      }
    }
    const std::vector<Node<T> *> & getChild() const { return (_child); };
    Node<T>*  getParent() const { return (_parent); };

  private:

    Node();
    Node(const Node &);
    Node & operator=(const Node<T> & );

};

#endif
