#include "webserv.hpp"

bool ConfigValidator::_valid = true;
bool ConfigValidator::_checked = false;
std::string ConfigValidator::_status = "ok";

ConfigValidator::ConfigValidator() {}
ConfigValidator::~ConfigValidator() {}

bool	ConfigValidator::validate(Node<Token> *node)
{

	if (!_valid)
		return (_valid);

	_checked = false;

	checkHttp(node);
	checkServer(node);
	checkLocation(node);
	checkListen(node);
	checkRoot(node);
	checkIndex(node);
	checkAutoindex(node);
	checkErrorPage(node);
	checkClientMaxBodySize(node);
	checkKeepaliveTimeout(node);
	checkServerName(node);
	checkCgi(node);
	checkAllowedMethod(node);

	if (!_checked && !(!node->getParent() && node->getName() == "base"))
		throw std::runtime_error("Unknown directive `" + node->getName() + "`");

	for(size_t i=0;i<node->getChild().size();++i)
			validate(node->getChild()[i]);
	return (_valid);

}

void ConfigValidator::checkHttp(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "http")
		return ;

	_checked = true;
	
	Directive http(node);
	http.acceptParent("base")
		.reqChild();

	_valid = DirectiveChecker::check(http);

}
void ConfigValidator::checkServer(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "server")
		return ;
	
	_checked = true;
	
	Directive server(node);
	server.acceptParent("http base")
	.reqChild();

	_valid = DirectiveChecker::check(server);

}
void ConfigValidator::checkLocation(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "location")
		return ;

	_checked = true;

	Directive location(node);

	location.acceptParent("server")
	.argCount(1, 1).argType("string");

	_valid = DirectiveChecker::check(location);
	if (!_valid)
		return ;

	std::string path = node->getData()[0].value;
	if (path.empty() || path.find('/') != 0)
		_valid = false;

}
void ConfigValidator::checkListen(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "listen")
		return ;

	_checked = true;

	Directive listen(node);

	listen.acceptParent("server")
	.argCount(1, 2).argType("number string ip");

	_valid = DirectiveChecker::check(listen);

	// @todo check valid port/ip

}
void ConfigValidator::checkRoot(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "root")
		return ;

	_checked = true;

	Directive root(node);
	root.acceptParent("server location")
	.argCount(1, 1);

	_valid = DirectiveChecker::check(root);

}
void ConfigValidator::checkIndex(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "index")
		return ;

	_checked = true;

	Directive index(node);
	index.acceptParent("server location")
	.argCount(1, -1).argType("string");

	_valid = DirectiveChecker::check(index);

}
void ConfigValidator::checkAutoindex(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "autoindex")
		return ;

	_checked = true;

	Directive autoindex(node);
	autoindex.acceptParent("server location")
	.argCount(1, 1).argType("on off");

	_valid = DirectiveChecker::check(autoindex);

}
void ConfigValidator::checkErrorPage(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "error_page")
		return ;

	_checked = true;
	
	Directive error_page(node);
	error_page.acceptParent("server location")
	.argCount(1, -1).argType("number string");

	_valid = DirectiveChecker::check(error_page);

	const std::vector<Token> &arg = node->getData();
	
	TokenType type = TokenU::inferType(arg[arg.size() - 1].value);
	if (type != TK_STRING)
		throw std::runtime_error("invalid error_page location");

}
void ConfigValidator::checkClientMaxBodySize(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "client_max_body_size")
		return ;

	_checked = true;

	Directive max_body(node);
	max_body.acceptParent("server location")
	.argCount(1, 1).argType("number");

	_valid = DirectiveChecker::check(max_body);

}
void ConfigValidator::checkKeepaliveTimeout(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "keepalive_timeout")
		return ;

	_checked = true;

	Directive keepalive(node);
	keepalive.acceptParent("http server")
	.argCount(1, 1).argType("number");

	_valid = DirectiveChecker::check(keepalive);

}
void ConfigValidator::checkServerName(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "server_name")
		return ;

	_checked = true;

	Directive server_name(node);
	server_name.acceptParent("server")
	.argCount(1, 1).argType("string");

	_valid = DirectiveChecker::check(server_name);

}
void ConfigValidator::checkCgi(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "cgi")
		return ;

	_checked = true;

	Directive cgi(node);
	cgi.acceptParent("location")
	.argCount(2, 2).argType("string");

	_valid = DirectiveChecker::check(cgi);

}
void ConfigValidator::checkAllowedMethod(Node<Token> * node)
{

	if (!node || !_valid || _checked)
		return ;

	if (node->getName() != "allowed_method")
		return ;

	_checked = true;

	Directive allowed_method(node);
	allowed_method.acceptParent("location")
	.argCount(0, -1).argType("string");

	_valid = DirectiveChecker::check(allowed_method);

}
