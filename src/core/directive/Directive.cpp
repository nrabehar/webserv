#include  "webserv.hpp"

Directive::Directive()
	: _node(NULL), _name(""), _min_arg(0),
		_max_arg(0), _req_child(false)
{
	_arg_type.push_back("string");
	_parent.push_back("");
}

Directive::Directive(Node<Token> * node)
	: _node(node), _name(node->getName()), _min_arg(0),
		_max_arg(0), _req_child(false)
{
	_arg_type.push_back("string");
	_parent.push_back("");
}

Directive::Directive(const Directive & other)
	: _node(other._node), _name(other._name), _min_arg(other._min_arg),
		_max_arg(other._max_arg), _arg_type(other._arg_type),
		_parent(other._parent), _req_child(other._req_child) {}

Directive & Directive::operator=(const Directive & other)
{
	if (this == &other)
		return (*this);
	_node = other._node;
	_name = other._name;
	_min_arg = other._min_arg;
	_max_arg = other._max_arg;
	_arg_type = other._arg_type;
	_parent = other._parent;
	_req_child = other._req_child;
	return (*this);
}

Directive::~Directive() {}

const std::string & Directive::name() const { return (_name); }
int Directive::min_arg() const { return (_min_arg); }
int Directive::max_arg() const { return (_max_arg); }
const std::vector<std::string> & Directive::parent() const { return (_parent); }
const std::vector<std::string> & Directive::arg_type() const { return (_arg_type); }
bool Directive::req_child() const { return (_req_child); }
Node<Token> * Directive::node() { return (_node); }
const Node<Token> * Directive::node() const { return (_node); }

Directive& Directive::argCount(int min, int max)
{

	_min_arg = min;
	_max_arg = max;

	return (*this);

}

Directive& Directive::argType(const std::string& types)
{
	_arg_type = split(types, ' ');
	return (*this);
}

Directive& Directive::acceptParent(const std::string & parent)
{
	_parent = split(parent, ' ');
	return (*this);
}

Directive& Directive::reqChild()
{
	_req_child = true;
	return (*this);
}

bool Directive::isAllowedIn(const std::string & name) const
{
	return (contains(_parent, name));
}

bool Directive::accept(const Directive & directive) const
{
	return (directive.isAllowedIn(_name));
}

std::vector<std::string> Directive::split(const std::string& s, char delim)
{

	std::vector<std::string> elems;

	std::string::size_type start = 0, end = 0;

	while ((end = s.find(delim, start)) != std::string::npos)
	{

		if (end > start) elems.push_back(s.substr(start, end - start));
      start = end + 1;

	}

	if (start < s.size())
		elems.push_back(s.substr(start));

	return (elems);

}

bool Directive::contains(const std::vector<std::string>& vec, const std::string& val) const
{

	for (size_t i = 0; i < vec.size(); ++i)
	{

		if (vec[i] == val) return true;

	}

	return (false);

}
