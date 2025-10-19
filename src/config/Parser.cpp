#include "webserv.hpp"

std::vector<Token>Config::Parser::_token;
size_t Config::Parser::_pos = 0;

Config::Parser::Parser() {}
Config::Parser::~Parser() {}

Node<std::string>*  Config::Parser::parse(std::vector<Token>& token)
{
  _token = token;
  Node<std::string>*  root = ft::alloc<Node<std::string> >("base");
  while (_token[_pos].type != TK_EOF)
    root->addChild(parseStatement());
  _token.clear();
  _pos = 0;
  return (root);
}

Node<std::string>* Config::Parser::parseStatement()
{
  expectType(TK_STRING);
  std::string name = _token[_pos].value;
  int         line = _token[_pos].line;

  std::vector<std::string> args;
  while ((_token[_pos].type & TK_STRING) && line == _token[_pos].line)
  {
    args.push_back(_token[_pos].value);
    ++_pos;
  }
  if (_token[_pos].type & TK_SYMBOL)
  {
    std::string value = _token[_pos].value;
    if (value == "#")
      return (skipComment(name, args));
    if (value == ";")
      return (parseDirective(name, args));
    else if (value == "{")
      return (parseBlock(name, args));
    else
      throw std::runtime_error("Syntax error at line "+toStr(_token[_pos].line));
  }
  else
    throw std::runtime_error("Syntax error at line "+toStr(_token[_pos].line));
}

Node<std::string>*  Config::Parser::parseDirective(const std::string & name, const std::vector<std::string> & args)
{
  Node<std::string>*  directive = ft::alloc<Node<std::string> >(name);
  for (size_t i = 0; i < args.size(); ++i)
    directive->push(args[i]);
  ++_pos;
  return (directive);
}

Node<std::string>*  Config::Parser::parseBlock(const std::string &name, const std::vector<std::string> &args)
{
  Node<std::string>*  block = ft::alloc<Node<std::string> >(name);
  ++_pos;

  for (size_t i = 0; i < args.size(); ++i)
    block->push(args[i]);
  while (!(_token[_pos].type & TK_SYMBOL) && _token[_pos].value != "}")
    block->addChild(parseStatement());

  ++_pos;
  return (block);
}

Node<std::string>*  Config::Parser::skipComment(const std::string &name, const std::vector<std::string> &args)
{
  int line = _token[_pos].line;
  while (_pos < _token.size() && line == _token[_pos].line)
    ++_pos;
  if ((_token[_pos].type & TK_SYMBOL) && _token[_pos].value == "#")
    return (skipComment(name, args));
  if (args.size())
  {
    Node<std::string>* directive = ft::alloc<Node<std::string> >(name);
    for (size_t i = 0; i < args.size(); ++i)
      directive->push(args[i]);
    return (directive);
  }
  return (NULL);
}

bool  Config::Parser::expectType(TokenType type)
{
  if (!(_token[_pos].type & type))
    throw std::runtime_error("Unexpected token at line " + toStr(_token[_pos].line));
  return (true);
}

std::string Config::Parser::toStr(int n)
{
  std::ostringstream oss;

  oss << n;
  return (oss.str());
}
