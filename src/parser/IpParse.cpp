#include "webserv.hpp"

IpParse::IpParse(std::istream & is) :
	_is(is),
	_host(""),
	_port(0)
{
}

IpParse::~IpParse() {
}

const std::string & IpParse::getHost() const {
	return (this->_host);
}

int IpParse::getPort() const {
	return (this->_port);
}

std::string IpParse::str() const {
	return ("");
}

void IpParse::parse() const {
	std::string line;
	std::getline(this->_is, line);

	if (line.empty())
		return;

	// @todo : add Utils function to trim string
	size_t start = line.find_first_not_of(" \t\n\r");
	if (start == std::string::npos)
		return;
	size_t end = line.find_last_not_of(" \t\n\r");
	line = line.substr(start, end - start + 1);

	this->_host = this->_extractIpHost(line);
	this->_port = this->_extractIpPort(line);
}

void IpParse::visit(const IData & data) const {
	const Ip * ip = dynamic_cast<const Ip *>(&data);
	if (!ip)
		return ;
	this->parse();
}

std::string IpParse::_extractIpHost(const std::string & hp) const {
	if (hp.empty())
		return ("");
	if (hp.find_first_not_of("0123456789") == std::string::npos &&
		hp.length() >= 1 &&
		hp.length() <= 5)
		return ("localhost");
	size_t cpos = hp.find(':');
	if (cpos == std::string::npos || cpos != hp.rfind(':'))
		return ("");
	return (hp.substr(0, cpos));
}

int IpParse::_extractIpPort(const std::string & hp) const {
	int port = 0;
	if (hp.empty())
		return (-1);
	if (hp.find_first_not_of("0123456789") == std::string::npos &&
		hp.length() >= 1 &&
		hp.length() <= 5)
	{
		std::istringstream pstream(hp);
		if (!(pstream >> port) || !pstream.eof() || port > 65535)
			return (-1);
		return (port);
	}
	size_t cpos = hp.find(':');
	if (cpos == std::string::npos || cpos != hp.rfind(':'))
		return (-1);
	std::istringstream pstream(hp.substr(cpos + 1));
	if (!(pstream >> port) || !pstream.eof() || port > 65535)
		return (-1);
	return (port);
}
