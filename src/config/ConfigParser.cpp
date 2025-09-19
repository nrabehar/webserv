#include "webserv.hpp"

std::vector<Token>	ConfigParser::_token;
size_t ConfigParser::_pos = 0;

ConfigParser::~ConfigParser() {}

ConfigNode*	ConfigParser::parse(std::vector<Token>& token)
{
	_token = token;
	ConfigNode*	root = new ConfigNode("root");
	while (_token[_pos].type != TK_EOF)
		root->addChild(parseStatement());
	_token.clear();
	_pos = 0;
	return (root);
}

ConfigNode* ConfigParser::parseStatement()
{
	expectType(TK_STRING);
	std::string name = _token[_pos].value;
	int					line = _token[_pos].line;
	++_pos;

	std::vector<std::string> args;
	while ((_token[_pos].type & TK_STRING) && line == _token[_pos].line)
		args.push_back(_token[_pos++].value);
	if((_token[_pos].type & TK_SYMBOL) && _token[_pos].value==";") {
		return (parseDirective(name, args));
	} else if((_token[_pos].type & TK_SYMBOL) && _token[_pos].value=="{")
		return (parseBlock(name, args));
	else
		throw std::runtime_error("Syntax error at line "+toStr(_token[_pos].line));
}

ConfigNode*	ConfigParser::parseDirective(const std::string &name, const std::vector<std::string> &args)
{
	ConfigNode*	directive = new ConfigNode(name);
	for (size_t i = 0; i < args.size(); ++i)
	directive->pushArg(args[i]);
	++_pos;
	return (directive);
}

ConfigNode*	ConfigParser::parseBlock(const std::string &name, const std::vector<std::string> &args)
{
	ConfigNode*	block = new ConfigNode(name);
	++_pos;

	for (size_t i = 0; i < args.size(); ++i)
		block->pushArg(args[i]);
	while (!(_token[_pos].type & TK_SYMBOL) && _token[_pos].value != "}")
		block->addChild(parseStatement());
	
	++_pos;
	return (block);
}

bool	ConfigParser::expectType(TokenType type)
{
	if (!(_token[_pos].type & type))
		throw std::runtime_error("Unexpected token at line " + toStr(_token[_pos].line));
	return (true);
}

std::string	ConfigParser::toStr(int n)
{
	std::ostringstream oss;

	oss << n;
	return (oss.str());
}
