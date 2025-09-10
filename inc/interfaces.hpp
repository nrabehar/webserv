#ifndef INTERFACES_HPP
#define INTERFACES_HPP 1

#include "webserv.hpp"

class IError
{
	public:
		virtual ~IError() {};
		virtual void reportError(EErrorCode) = 0;
};

class IChecker
{
	public:
		virtual ~IChecker() {};
		virtual ECheck check() const = 0;
};

class IParser
{
	public:
		virtual ~IParser() {};
		virtual void parse(const std::string & = "") = 0;
};

class ILoader
{
	public:
		virtual ~ILoader() {};
		virtual bool load() = 0;
};

class IHandler
{
	public:
		virtual ~IHandler() {};
		virtual bool handle() = 0;
};

#endif // INTERFACES_HPP
