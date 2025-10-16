#include "webserv.hpp"

ATokenExtractor::ATokenExtractor() : _next(NULL) {}
ATokenExtractor::~ATokenExtractor()
{
	if (_next)
		delete _next;
}
TokenType	ATokenExtractor::getType(const std::string &v)
{
	if (_next)
		return (_next->getType(v));
	if (v.empty())
		return (TK_EOF);
	return (TK_STRING);
}
ITokenTypeExtractor *	ATokenExtractor::next(ITokenTypeExtractor * ext)
{
	_next = ext;
	return (_next);
}

TokenExtractor::TokenExtractor(): ATokenExtractor() {}
TokenExtractor::~TokenExtractor(){}
TokenType	TokenExtractor::getType(const std::string &v) { return (ATokenExtractor::getType(v)); }

TokenSymboleExtractor::TokenSymboleExtractor(): ATokenExtractor() {}
TokenSymboleExtractor::~TokenSymboleExtractor(){}
TokenType	TokenSymboleExtractor::getType(const std::string &v)
{
	if (v.size() != 1 || !TokenU::isSymbol(v))
		return (ATokenExtractor::getType(v));
	int c = v[0];
	switch (c)
	{
		case ';': return (TK_SEMICOLON);
		case '#': return (TK_COMMENT);
		case '{': return (TK_BRACE_O);
		case '}': return (TK_BRACE_C);
		default: return (TK_SYMBOL);
	}
}


TokenNumberExtractor::TokenNumberExtractor(): ATokenExtractor() {}
TokenNumberExtractor::~TokenNumberExtractor(){}
TokenType	TokenNumberExtractor::getType(const std::string &v)
{
	if (v.empty())
		return (ATokenExtractor::getType(v));
	std::string s = "0123456789";
	if (v.find_first_not_of(s) == std::string::npos)
		return (TK_NUMBER);
	return (ATokenExtractor::getType(v));
}

TokenBoolExtractor::TokenBoolExtractor(): ATokenExtractor() {}
TokenBoolExtractor::~TokenBoolExtractor(){}
TokenType	TokenBoolExtractor::getType(const std::string &v)
{
	if (v == "true" || v == "false")
		return (TK_BOOLEAN);
	return (ATokenExtractor::getType(v));
}

TokenOnOffExtractor::TokenOnOffExtractor(): ATokenExtractor() {}
TokenOnOffExtractor::~TokenOnOffExtractor(){}
TokenType	TokenOnOffExtractor::getType(const std::string &v)
{
	if (v == "on")
		return (TK_ON);
	if (v == "off")
		return (TK_OFF);
	return (ATokenExtractor::getType(v));
}

TokenSizeExtractor::TokenSizeExtractor(): ATokenExtractor() {}
TokenSizeExtractor::~TokenSizeExtractor(){}
TokenType	TokenSizeExtractor::getType(const std::string &v)
{
	if (v.empty())
		return (ATokenExtractor::getType(v));
	size_t pos = v.find_first_not_of("0123456789");
	if (pos == std::string::npos)
		return (TK_NUMBER);
	std::string num = v.substr(0, pos);
	std::string unit = v.substr(pos);
	if (num.empty() || !String::isNumeric(num))
		return (ATokenExtractor::getType(v));
	if (unit == "K" || unit == "M" || unit == "G" ||
	    unit == "k" || unit == "m" || unit == "g" ||
	    unit.empty())
		return (TK_SIZE);
	return (ATokenExtractor::getType(v));
}

