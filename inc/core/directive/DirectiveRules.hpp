#ifndef DIRECTIVERULES_HPP
#define DIRECTIVERULES_HPP

#include "../../webserv.hpp"
#include "Directive.hpp"

class DirectiveRules {
	
	public:

		~DirectiveRules();
  
		static void checkHttp(Directive& d);
    static void checkServer(Directive& d);
    static void checkLocation(Directive& d);
    static void checkListen(Directive& d);
    static void checkRoot(Directive& d);
    static void checkIndex(Directive& d);
    static void checkAutoindex(Directive& d);
    static void checkErrorPage(Directive& d);
    static void checkClientMaxBodySize(Directive& d);
    static void checkKeepaliveTimeout(Directive& d);
    static void checkServerName(Directive& d);
    static void checkCgi(Directive& d);
    static void checkAllowedMethod(Directive& d);

    static void build(Directive& d);

	

};

#endif // DIRECTIVERULES_HPP
