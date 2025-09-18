#include "webserv.hpp"

Config::Config():
_content("")
{
}

Config::~Config()
{
}

Config::Config(const Config &):
_content("")
{
}

Config & Config::operator=(const Config & src)
{
	if (this == &src)
		return (*this);
	this->setContent(src.getContent());
	return (*this);
}

Config & Config::operator=(const IConfig & src)
{
	if (this == &src)
		return (*this);
	this->setContent(src.getContent());
	return (*this);
}

const std::string & Config::getContent() const
{
	return (this->_content);
}

void Config::setContent(const std::string & content)
{
	this->_content = content;
}
