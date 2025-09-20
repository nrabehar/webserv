#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "../webserv.hpp"

enum TokenType
{
	TK_NONE,
	TK_STRING  = 1 << 0,
	TK_SYMBOL  = 1 << 1,
	TK_EOL     = 1 << 2,
	TK_EOF     = 1 << 3
};

struct Token
{
	std::string	value;
	int					line;
	TokenType 	type;

	Token(const std::string & v, int l, TokenType t = TK_NONE);
	bool				isType(TokenType t) const;
};


class TokenU
{

	private:

		static std::string _c_s;
		static std::string _symbol;

	public:

		~TokenU();
		static void				useSymbole(const std::string &c_s);
		static TokenType	inferType(const std::string & t);
		static std::string typeToString(TokenType t);
	
	private:
	
		TokenU();
		TokenU(const TokenU &);
		TokenU & operator=(const TokenU &);

		static bool isSymbol(const std::string & v);

};

class ITokenStream
{

	public:

		virtual ~ITokenStream() {}
    virtual const Token& peek() const = 0;
    virtual const Token& next() = 0;
    virtual void skip() = 0;
    virtual bool eof() const = 0;

};

class TokenStream
	: virtual public ITokenStream
{

	private:

		std::vector<Token> _tokens;
		size_t						 _cursor;

	public:

		explicit TokenStream(const std::vector<Token>& tokens);
		explicit TokenStream(const TokenStream &);
		TokenStream & operator=(const TokenStream &);
		virtual ~TokenStream();

		const Token& peek() const;
    const Token& next();
    void skip();
    bool eof() const;

	private:
		
		TokenStream();

};




#endif // TOKEN_HPP