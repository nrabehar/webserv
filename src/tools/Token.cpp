#include "webserv.hpp"

std::string TokenU::_c_s = "";
std::string TokenU::_symref = "!@#$%^&*()_-+={}[]\\|/~";

Token::Token(const std::string & v, int l, TokenType t)
	: value(v), line(l), type(t)
{
	if (type == TK_NONE)
		type = TokenU::inferType(v);
}

Token::Token(const std::string & v, int l)
	: value(v), line(l), type(TK_NONE)
	{ type = TokenU::inferType(v); }

bool	Token::isType(TokenType t) const { return (type == t); }

TokenU::~TokenU() {}
void	TokenU::setSymRef(const std::string &c_s) {_c_s = c_s; }
void	TokenU::resetSymRef() {_c_s.clear(); }
std::string TokenU::getSymRef() { return (_c_s.empty() ? _symref : _c_s); }
bool	TokenU::isSymbol(const std::string& v)
{
	int c = v[0];
	if (v.size() != 1)
		return (false);
	std::string used_s = _symref;
	if (!_c_s.empty())
		used_s = _c_s;
	return (used_s.find(c) != std::string::npos);
}

TokenType	TokenU::inferType(const std::string & t)
{
	TokenExtractor *extractor = new TokenExtractor();

	extractor->next(new TokenSymboleExtractor())
					->next(new TokenNumberExtractor());

	TokenType type = extractor->getType(t);
	delete extractor;
	return (type);
}

std::string TokenU::typeToString(TokenType t)
{
	switch (t)
	{
		case TK_NONE:   return "none";
		case TK_SYMBOL: return "symbol";
		case TK_BRACE_O: return "open_brace";
		case TK_BRACE_C: return "close_brace";
		case TK_COMMA:	return "comma";
		case TK_COMMENT: return "comment";
		case TK_STRING: return "string";
		case TK_NUMBER: return "number";
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
const Token& TokenStream::next() { return (_tokens[++_cursor]); }
void TokenStream::skip() {
	std::cout << "Skip [" << _tokens[_cursor].value << "] at line " << _tokens[_cursor].line;
	_tokens[_cursor++];
	std::cout << " => [" << _tokens[_cursor].value  << "] at line " << _tokens[_cursor].line << std::endl;
}
bool	TokenStream::eof() const { return (_cursor >= _tokens.size() || _tokens[_cursor].isType(TK_EOF)); } 
