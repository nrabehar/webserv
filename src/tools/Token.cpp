#include "webserv.hpp"

std::string TokenU::_c_s = "";
std::string TokenU::_symbol = "!@#$%^&*()_-+={}[]\\|/~";

Token::Token(const std::string & v, int l, TokenType t)
	: value(v), line(l), type(t)
{
	if (type == TK_NONE)
		type = TokenU::inferType(v);
}

bool	Token::isType(TokenType t) const { return (type & t); }

void	TokenU::useSymbole(const std::string &c_s) {_c_s = c_s; }
bool	TokenU::isSymbol(const std::string& v)
{
	int c = v[0];
	if (v.size() != 1)
		return (false);
	std::string used_s = _symbol;
	if (!_c_s.empty())
		used_s = _c_s;
	return (used_s.find(c) != std::string::npos);
}

TokenType	TokenU::inferType(const std::string & t)
{
	if (t.empty())	return (TK_NONE);
	if (isSymbol(t)) return (TK_SYMBOL);
	return (TK_STRING);
}

std::string TokenU::typeToString(TokenType t)
{
	switch (t)
	{
		case TK_NONE:   return "none";
		case TK_STRING: return "string";
		case TK_SYMBOL: return "symbol";
		case TK_EOL:    return "eol";
		case TK_EOF:    return "eof";
		default:        return "unknown";
	}
}

TokenStream::TokenStream(const std::vector<Token> & tokens)
	: _tokens(tokens), _cursor(0) {}
TokenStream::TokenStream(const TokenStream &other)
	: _tokens(other._tokens), _cursor(other._cursor) {}
TokenStream & TokenStream::operator=(const TokenStream &other)
{
	_tokens = other._tokens;
	_cursor = other._cursor;
	return (*this);
}
TokenStream::~TokenStream() {}
const Token& TokenStream::peek() const { return (_tokens[_cursor]); }
const Token& TokenStream::next() { return (_tokens[_cursor++]); }
void TokenStream::skip() {
	std::cout << "Skipping " << _tokens[_cursor].value << " at line " << _tokens[_cursor].line;
	_tokens[_cursor++];
	std::cout << " and got " << _tokens[_cursor].value  << " at line " << _tokens[_cursor].line << std::endl;
}
bool	TokenStream::eof() const { return (_cursor >= _tokens.size() || _tokens[_cursor].isType(TK_EOF)); } 
