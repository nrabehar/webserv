#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "../webserv.hpp"

enum TokenType
{
	TK_NONE,
	TK_SYMBOL  		= 1 << 1,
	TK_BRACE_O 		= 1 << 2,
	TK_BRACE_C 		= 1 << 3,
	TK_SEMICOLON	= 1 << 4,
	TK_COMMENT	 	= 1 << 5,

	TK_STRING	 		= 1 << 6,
	TK_NUMBER	 		= 1 << 7,
	TK_BOOLEAN 		= 1 << 8,
	TK_SIZE	 		= 1 << 9,

	TK_ON 			= 1 << 10,
	TK_OFF 			= 1 << 11,

	TK_EOL 			= 1 << 12,
	TK_EOF     		= 1 << 13
};

struct Token
{
	std::string	value;
	int					line;
	TokenType 	type;

	Token(const std::string & v, int l, TokenType t);
	Token(const std::string & v, int l);
	bool				isType(TokenType t) const;
};


class TokenU
{

	private:

		static std::string _c_s;
		static std::string _symref;

	public:

		~TokenU();
		static void				setSymRef(const std::string &c_s);
		static void				resetSymRef();
		static std::string getSymRef();
		static TokenType	inferType(const std::string & t);
		static std::string typeToString(TokenType t);
		static TokenType strToType(const std::string & t);
		static bool isSymbol(const std::string & v);
	
	private:
	
		TokenU();
		TokenU(const TokenU &);
		TokenU & operator=(const TokenU &);


};

class ITokenStream
{

	public:

		virtual ~ITokenStream() {}
    virtual const Token& peek() const = 0;
    virtual const Token& prev() const = 0;
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
    const Token& prev() const;
    void skip();
    bool eof() const;

	private:
		
		TokenStream();

};




#endif