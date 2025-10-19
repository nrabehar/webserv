#include "webserv.hpp"

DirectiveParser::DirectiveParser(ITokenStream & stream)
  : _stream(stream) {};
DirectiveParser::~DirectiveParser() {}

Node<Token> * DirectiveParser::parse()
{

  Node<Token> * base = ft::alloc<Node<Token> >("base");

  try
  {

    while (!_stream.eof())
      base->addChild(parseStatement());

    return (base);

  }
  catch(const std::exception& e)
  {

    ft::free(base);

    throw;

  }

};

Node<Token> * DirectiveParser::parseStatement()
{

  std::vector<Token> args;

  if (_stream.eof())
    return (NULL);

  Token t = _stream.peek();
  std::string node_name = t.value;

  if (!t.isType(TK_STRING))
    throw std::runtime_error("Unexpected token `" + _stream.peek().value + "` at line " + line());

  t = _stream.next();

  while (t.type & (TK_STRING | TK_NUMBER | TK_SIZE | TK_ON | TK_OFF))
  {
    args.push_back(t);
    t = _stream.next();
  }

  if (t.isType(TK_SEMICOLON))
    return (parseDirective(node_name, args));

  if (t.isType(TK_BRACE_O))
    return (parseBlock(node_name, args));

  return (parseStatement());

}

Node<Token>*  DirectiveParser::parseDirective(const std::string & name, const std::vector<Token> & arg)
{

  Node<Token> * directive = ft::alloc<Node<Token> >(name);

  for (size_t i = 0; i < arg.size() ; ++i)
    directive->push(arg[i]);

  _stream.skip();

  return (directive);

}

Node<Token>*  DirectiveParser::parseBlock(const std::string & name, const std::vector<Token> & arg)
{

  Node<Token> * block = NULL;

  _stream.skip();

  if (_stream.eof())
    throw std::runtime_error("Unexpected end of file");

  Token t = _stream.peek();

  if (!t.isType(TK_STRING) && !t.isType(TK_BRACE_C))
    throw std::runtime_error("Unexpected `" + t.value + "` at line " + line());

  block = ft::alloc<Node<Token> >(name);

  for (size_t i = 0; i < arg.size(); ++i)
    block->push(arg[i]);

  while (!_stream.eof() && !_stream.peek().isType(TK_BRACE_C))
    block->addChild(parseStatement());

  if (_stream.eof())
    throw std::runtime_error("Unexpected end of file last line " + line());

  _stream.skip();

  return (block);

}

std::string DirectiveParser::line()
{

  std::ostringstream oss;

  if (_stream.eof())
    oss << _stream.prev().line;
  else
    oss << _stream.peek().line;

  return (oss.str());

}
