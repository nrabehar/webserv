#include "webserv.hpp"

IpValid::IpValid() :
	_isvalid(false)
{
}

IpValid::~IpValid() {
}

std::string IpValid::str() const {
	return ("");
}

bool IpValid::valid() const {
	return (this->_isvalid);
}

void IpValid::visit(const IData & data) const {
	const Ip * ip = dynamic_cast<const Ip *>(&data);
	if (!ip)
		return ;
	_isvalid =
		this->_isValidIpHost(ip->getHost()) &&
		this->_isValidIpPort(ip->getPort());
}

bool IpValid::_isValidIpHost(const std::string & host) const {
	unsigned int a, b, c, d;
	char dot1, dot2, dot3;
	if (host.empty())
		return (false);
	if (host == "localhost")
		return (true);
	std::istringstream hstream(host);
	if (!(hstream >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d) ||
		dot1 != '.' || dot2 != '.' || dot3 != '.' ||
		a > 255 || b > 255 || c > 255 || d > 255 ||
		!hstream.eof())
		return (false);
	return (true);
}

bool IpValid::_isValidIpPort(int port) const {
	return (port > 0 && port < 65536);
}
