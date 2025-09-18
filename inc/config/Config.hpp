#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../webserv.hpp"

class Config:
public virtual IConfig
{

	private:

		std::string _content;

	public:

		Config();
		virtual ~Config();
		Config(const Config &);
		Config & operator=(const Config &);

		Config & operator=(const IConfig &);

		const std::string & getContent() const;
		void setContent(const std::string &);
};

#endif
