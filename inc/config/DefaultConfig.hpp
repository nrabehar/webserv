#ifndef DEFAULTCONFIG_HPP
#define DEFAULTCONFIG_HPP

#include "../webserv.hpp"


class DefaultConfig:
public virtual IConfig
{

	private:

		std::string _content;

	public:

		DefaultConfig();
		virtual ~DefaultConfig();
		DefaultConfig(const DefaultConfig &);
		DefaultConfig & operator=(const DefaultConfig &);

		DefaultConfig & operator=(const IConfig &);

		virtual const std::string & getContent() const;
		virtual void setContent(const std::string &);

};


#endif
