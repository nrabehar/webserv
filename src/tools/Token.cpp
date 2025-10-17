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

bool	Token::isType(TokenType t) const { return (type & t); }

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
					->next(new TokenNumberExtractor())
					->next(new TokenBoolExtractor())
					->next(new TokenOnOffExtractor())
					->next(new TokenSizeExtractor());

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
		case TK_SEMICOLON:	return "semicolon";
		case TK_COMMENT: return "comment";
		case TK_STRING: return "string";
		case TK_NUMBER: return "number";
		case TK_BOOLEAN: return "boolean";
		case TK_SIZE: return "size";
		case TK_ON: return "on";
		case TK_OFF: return "off";
		case TK_EOF:    return "eof";
		default:        return "unknown";
	}
}

TokenType TokenU::strToType(const std::string & t)
{
	if (t == "none")        return TK_NONE;
	if (t == "symbol") return TK_SYMBOL;
	if (t == "open_brace")  return TK_BRACE_O;
	if (t == "close_brace") return TK_BRACE_C;
	if (t == "semicolon")   return TK_SEMICOLON;
	if (t == "comment") return TK_COMMENT;
	if (t == "string")  return TK_STRING;
	if (t == "number")  return TK_NUMBER;
	if (t == "boolean") return TK_BOOLEAN;
	if (t == "size")    return TK_SIZE;
	if (t == "on") return TK_ON;
	if (t == "off") return TK_OFF;
	if (t == "eof")     return TK_EOF;

	return TK_NONE;
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
const Token& TokenStream::prev() const { return (_tokens[_cursor - 1]); }
void TokenStream::skip() { _tokens[_cursor++]; }
bool	TokenStream::eof() const { return (_cursor >= _tokens.size() || _tokens[_cursor].isType(TK_EOF)); }
