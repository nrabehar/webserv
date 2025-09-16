#ifndef IPPARSE_HPP
#define IPPARSE_HPP

#include "../webserv.hpp"

class IpParse :
	public IIpParse
{

	protected:

		std::istream &			_is;
		mutable std::string	_host;
		mutable int					_port;

	public:

		IpParse(std::istream &);
		virtual ~IpParse();

		const std::string & getHost() const;
		int getPort() const;

		virtual std::string str() const;

		virtual void parse() const;

		virtual void visit(const IData &) const;

	private:

		virtual std::string _extractIpHost(const std::string &) const;
		virtual int _extractIpPort(const std::string &) const;

		IpParse();
		IpParse(const IpParse &);
		IpParse & operator=(const IpParse &);

};

#endif
