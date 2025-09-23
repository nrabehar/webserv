#include "webserv.hpp"

bool Config::Validator::_valid = true;
bool Config::Validator::_checked = false;
std::string Config::Validator::_status = "ok";

Config::Validator::Validator() {}
Config::Validator::~Validator() {}

bool	Config::Validator::validate(Node<Token> *node)
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

void Config::Validator::checkHttp(Node<Token> * node)
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
void Config::Validator::checkServer(Node<Token> * node)
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
void Config::Validator::checkLocation(Node<Token> * node)
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
void Config::Validator::checkListen(Node<Token> * node)
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
void Config::Validator::checkRoot(Node<Token> * node)
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
void Config::Validator::checkIndex(Node<Token> * node)
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
void Config::Validator::checkAutoindex(Node<Token> * node)
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
void Config::Validator::checkErrorPage(Node<Token> * node)
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
void Config::Validator::checkClientMaxBodySize(Node<Token> * node)
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
void Config::Validator::checkKeepaliveTimeout(Node<Token> * node)
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
void Config::Validator::checkServerName(Node<Token> * node)
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
void Config::Validator::checkCgi(Node<Token> * node)
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
void Config::Validator::checkAllowedMethod(Node<Token> * node)
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
