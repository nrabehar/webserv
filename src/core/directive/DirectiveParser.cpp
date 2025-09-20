#include "webserv.hpp"

DirectiveParser::DirectiveParser(ITokenStream & stream)
	: _stream(stream) {
		if (!stream.eof())
			std::cout << "initialized with stream first " << stream.peek().value << std::endl;
		else
			std::cout << "Got " << TokenU::typeToString(TK_STRING) << std::endl;
	};
DirectiveParser::~DirectiveParser() {}

Node<Token> * DirectiveParser::parse()
{
	Node<Token> * base = new Node<Token>("base");
	while (!_stream.eof())
		base->addChild(parseStatement());
	return (base);
};

Node<Token> * DirectiveParser::parseStatement()
{
	std::vector<Token> mk;
	if (_stream.eof())
	{
		std::cout << "Finale end of line" << std::endl;
		return (NULL);
	}
	std::string name = _stream.peek().value;
	if (name == "#")
		skipComment(name, mk);
	if (!_stream.peek().isType(TK_STRING))
		throw std::runtime_error("Unexpected token at line " + lineStr());
	std::vector<Token> args;
	Token t = _stream.next();
	int curr_line = t.line;
	while (t.isType(TK_STRING) && t.line == curr_line)
	{
		std::cout << "Pushing arg " << t.value << " == "; 
		args.push_back(t);
		t = _stream.next();
		std::cout << "With next token " << t.value << std::endl;
	}
	std::cout << "Token  " << t.value << " after args " << std::endl;
	if (t.isType(TK_SYMBOL))
	{
		if (t.value == "#")
			return (skipComment(name, args));
		if (t.value == ";")
			return (parseDirective(name, args));
		if (t.value == "{")
			return (parseBlock(name, args));
		throw std::runtime_error("Syntax error at line " + lineStr());
	}
	throw std::runtime_error("Syntax error at line " + lineStr());
}

Node<Token>*	DirectiveParser::parseDirective(const std::string & name, const std::vector<Token> & arg)
{
	std::cout << "Enter to comma at line " << _stream.peek().line << std::endl;
	Node<Token> * directive = new Node<Token>(name);
	for (size_t i = 0; i < arg.size() ; ++i)
		directive->push(arg[i]);
	_stream.skip();
	return (directive);
}

Node<Token>*	DirectiveParser::parseBlock(const std::string & name, const std::vector<Token> & arg)
{
	Token t = _stream.peek();
	std::cout << "Enter to block at line " << t.line << " " << t.value << std::endl;
	_stream.skip();
	t = _stream.peek();
	std::cout << "Type of " << t.value << " " << TokenU::typeToString(t.type) << std::endl;
	if (!_stream.eof() && _stream.peek().isType(TK_SYMBOL))
		std::cout << "Should return for successed symbol" << std::endl;
	Node<Token> * block = new Node<Token>(name);
	for (size_t i = 0; i < arg.size(); ++i)
		block->push(arg[i]);
	while (!_stream.eof() && !_stream.peek().isType(TK_SYMBOL) && _stream.peek().value != "}")
		block->addChild(parseStatement());
	if (!_stream.eof())
		_stream.skip();
	return (block);
}

Node<Token>*	DirectiveParser::skipComment(const std::string & name, const std::vector<Token> & arg)
{
	std::cout << "Enter to comment at line " << _stream.peek().line << std::endl;
	int	curr_line = _stream.peek().line;
	while (!_stream.eof() && _stream.peek().line == curr_line)
		_stream.skip();
	if (!_stream.eof() && _stream.peek().isType(TK_SYMBOL) && _stream.peek().value == "#")
		return (skipComment(name, arg));
	if (arg.size())
	{
		Node<Token> * directive = new Node<Token>(name);
		for (size_t i = 0; i < arg.size() ; ++i)
			directive->push(arg[i]);
	}
	return (NULL);
}

std::string	DirectiveParser::lineStr()
{
	if (_stream.eof())
		return ("unknown");
	std::ostringstream oss;

	oss << _stream.peek().line;
	return (oss.str());
}
