#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "../../webserv.hpp"

#define DEFAULT_DIRECTIVE_PATH "conf/conf.directive"

class Directive
{

	private:

		Node<Token> * _node;
		std::string _name;
		int	_min_arg;
		int _max_arg;
		std::vector<std::string> _arg_type;
		std::vector<std::string> _parent;
		bool _req_child;

	public:

		Directive();
		Directive(Node<Token> * node);
		Directive(const Directive & other);
		Directive & operator=(const Directive & other);
		~Directive();

    Directive& argCount(int minArg, int maxArg);
    Directive& argType(const std::string& types);
    Directive& acceptParent(const std::string& parents);
    Directive& reqChild();

		const std::string & name() const;
		int min_arg() const;
		int max_arg() const;
		const std::vector<std::string> & parent() const;
		const std::vector<std::string> & arg_type() const;
		bool req_child() const;
		Node<Token> *	node();
		const Node<Token> *	node() const;

		bool isAllowedIn(const std::string & directive) const;
		bool accept(const Directive & node) const;

	private:

		std::vector<std::string> split(const std::string& s, char delim);
    bool contains(const std::vector<std::string>& vec, const std::string& val) const;

};


#endif // DIRECTIVE_HPP