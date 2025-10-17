#ifndef NETUTIL_HPP
#define NETUTIL_HPP

#include "String.hpp"

class NetUtil
{

	public:

		~NetUtil();

		static	bool	isValidPort(int port);
		static	bool	isValidPortStr(const std::string & p_str);
		static	bool	isValidIp(const std::string & ip);

	private:

		NetUtil();
		NetUtil(const NetUtil &);
		NetUtil & operator=(const NetUtil &);

};

#endif
