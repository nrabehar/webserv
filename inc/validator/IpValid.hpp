#ifndef IPVALID_HPP
#define IPVALID_HPP

#include "../webserv.hpp"

class IpValid :
	public IIpValid
{

	private:

		mutable bool	_isvalid;

	public:

		IpValid();
		virtual ~IpValid();

		virtual std::string str() const;

		virtual bool valid() const;

		virtual void visit(const IData &) const;

	private:

		virtual bool _isValidIpHost(const std::string &) const;
		virtual bool _isValidIpPort(int) const;

		IpValid(const IpValid &);
		IpValid & operator=(const IpValid &);

};

#endif
