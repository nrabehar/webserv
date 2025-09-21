#ifndef NODECHECKER_HPP
#define NODECHECKER_HPP

#include "../../webserv.hpp"

class NodeChecker
{

	public:

		~NodeChecker();

  	class Checker;

		static Checker check(const Node<Token>* node);

  private:

    NodeChecker();
    NodeChecker(const NodeChecker &);
    NodeChecker & operator=(const NodeChecker &);

};

class NodeChecker::Checker
{

	public:

    ~Checker();
		Checker(const Checker &);
    Checker(const Node<Token>* n);

    Checker& argCount(int minArg, int maxArg);
    Checker& argType(const std::string& types);
    Checker& acceptParent(const std::string& parents);
    Checker& requireChild();

    bool isValid() const;

	private:
    
    Checker();
		Checker & operator=(const Checker &); 
  	
		const Node<Token>* _node;
    bool _valid;

    static std::vector<std::string> split(const std::string& s, char delim);
    static bool contains(const std::vector<std::string>& vec, const std::string& val);
};

#endif // NODECHECKER_HPP
