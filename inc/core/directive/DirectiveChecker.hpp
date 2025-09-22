#ifndef DIRECTIVECHECKER_HPP
#define DIRECTIVECHECKER_HPP

#include "../../webserv.hpp"

class DirectiveChecker
{

  private:

    static bool _valid;

	public:

		~DirectiveChecker();
    static bool check(const Directive & directive);

  private:

    DirectiveChecker();
    DirectiveChecker(const DirectiveChecker &);
    DirectiveChecker & operator=(const DirectiveChecker &);

    static void argCount(const Directive & d);
    static void argType(const Directive & d);
    static void acceptParent(const Directive & d);
    static void requireChild(const Directive & d);

    static bool contains(const std::vector<std::string>& vec, const std::string& val);

};

#endif // DIRECTIVECHECKER_HPP
