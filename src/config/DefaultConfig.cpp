#include "webserv.hpp"

DefaultConfig::DefaultConfig():
_content("Default Config Content")
{
}

DefaultConfig::~DefaultConfig()
{
}

DefaultConfig::DefaultConfig(const DefaultConfig &):
_content("Default Config Content")
{
}

DefaultConfig & DefaultConfig::operator=(const DefaultConfig & src)
{
	if (this == &src)
		return (*this);
	this->setContent(src.getContent());
	return (*this);
}

DefaultConfig & DefaultConfig::operator=(const IConfig & src)
{
	if (this == &src)
		return (*this);
	this->setContent(src.getContent());
	return (*this);
}

const std::string & DefaultConfig::getContent() const
{
	return (this->_content);
}

void DefaultConfig::setContent(const std::string & content)
{
	this->_content = content;
}
