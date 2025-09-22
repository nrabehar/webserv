#include "webserv.hpp"

DirectiveParser::DirectiveParser(ITokenStream & stream)
	: _stream(stream) {};
DirectiveParser::~DirectiveParser() {}

Node<Token> * DirectiveParser::parse()
{
	
	Node<Token> * base = new Node<Token>("base");
	
	try
	{
	
		while (!_stream.eof())
			base->addChild(parseStatement());
	
		return (base);
	
	}
	catch(const std::exception& e)
	{
	
		delete base;
	
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
	
	if (t.isType(TK_COMMENT))
		skipComment("", std::vector<Token>());

	if (!t.isType(TK_STRING))
		throw std::runtime_error("Unexpected token `" + _stream.peek().value + "` at line " + line());

	t = _stream.next();

	while (t.isType(TK_STRING) || t.isType(TK_NUMBER) || t.isType(TK_ON) || t.isType(TK_OFF))
	{
		args.push_back(t);
		t = _stream.next();
	}
	
	if (t.isType(TK_COMMENT))
		return (skipComment(node_name, args));
	
	if (t.isType(TK_SEMICOLON))
		return (parseDirective(node_name, args));

	if (t.isType(TK_BRACE_O))
		return (parseBlock(node_name, args));
	
	return (parseStatement());

}

Node<Token>*	DirectiveParser::parseDirective(const std::string & name, const std::vector<Token> & arg)
{

	Node<Token> * directive = new Node<Token>(name);

	for (size_t i = 0; i < arg.size() ; ++i)
		directive->push(arg[i]);

	_stream.skip();

	return (directive);

}

Node<Token>*	DirectiveParser::parseBlock(const std::string & name, const std::vector<Token> & arg)
{

	Node<Token> * block = NULL;

	_stream.skip();
	
	if (_stream.eof())
		throw std::runtime_error("Unexpected end of file");
	
	Token t = _stream.peek();
	
	if (t.isType(TK_COMMENT))
		block = skipComment(name, arg);

	if (_stream.eof())
	{
		delete block;
		throw std::runtime_error("Unexpected end of file");
	}

	if (!_stream.eof() && !t.isType(TK_STRING) && !t.isType(TK_BRACE_C))
		throw std::runtime_error("Unexpected `" + t.value + "` at line " + line());

	if (!block)
	{

		block = new Node<Token>(name);
		
		for (size_t i = 0; i < arg.size(); ++i)
			block->push(arg[i]);
		
	}

	while (!_stream.eof() && !_stream.peek().isType(TK_BRACE_C))
		block->addChild(parseStatement());
	
	if (_stream.eof())
		throw std::runtime_error("Unexpected end of file");
		
	_stream.skip();

	return (block);

}

Node<Token>*	DirectiveParser::skipComment(const std::string & name, const std::vector<Token> & arg)
{

	int	curr_line = _stream.peek().line;

	while (!_stream.eof() && _stream.peek().line == curr_line)
		_stream.skip();

	if (!_stream.eof() && _stream.peek().isType(TK_COMMENT))
		return (skipComment(name, arg));

	if (arg.size())
	{

		Node<Token> * directive = new Node<Token>(name);
		for (size_t i = 0; i < arg.size() ; ++i)
			directive->push(arg[i]);
		return (directive);

	}

	return (NULL);

}

std::string	DirectiveParser::line()
{

	if (_stream.eof())
		return ("unknown");
	
	std::ostringstream oss;
	oss << _stream.peek().line;

	return (oss.str());

}
