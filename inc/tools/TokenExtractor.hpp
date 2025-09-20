#ifndef TOKENEXTRACTOR_HPP
#define TOKENEXTRACTOR_HPP

#include "../webserv.hpp"


class ITokenTypeExtractor
{

	public:

		virtual ~ITokenTypeExtractor() {}
		virtual TokenType getType(const std::string &value) = 0;
		virtual ITokenTypeExtractor *next(ITokenTypeExtractor * extractor) = 0;

};

class ATokenExtractor
	: public ITokenTypeExtractor
{

	protected:

		ITokenTypeExtractor *	_next;
		

	public:

		ATokenExtractor();
		~ATokenExtractor();
		ITokenTypeExtractor *next(ITokenTypeExtractor * extractor);
		TokenType getType(const std::string &value);

	private:

		ATokenExtractor(const ATokenExtractor &);
		ATokenExtractor & operator=(const ATokenExtractor &);

};


class TokenExtractor
	: public ATokenExtractor
{

	private:


	public:

		TokenExtractor();
		~TokenExtractor();
		TokenType getType(const std::string &value);

	private:

		TokenExtractor(const TokenExtractor &);
		TokenExtractor & operator=(const TokenExtractor &);

};

class TokenSymboleExtractor
	: public ATokenExtractor
{

	private:

		

	public:

		TokenSymboleExtractor();
		~TokenSymboleExtractor();
		TokenType getType(const std::string &value);

	private:

		TokenSymboleExtractor(const TokenSymboleExtractor &);
		TokenSymboleExtractor & operator=(const TokenSymboleExtractor &);

};


class TokenNumberExtractor
	: public ATokenExtractor
{

	private:

		

	public:

		TokenNumberExtractor();
		~TokenNumberExtractor();
		TokenType getType(const std::string &value);

	private:

		TokenNumberExtractor(const TokenNumberExtractor &);
		TokenNumberExtractor & operator=(const TokenNumberExtractor &);

};




#endif // TOKENEXTRACTOR_HPP