#include "webserv.hpp"

Ip::Ip() :
	_host("0.0.0.0"),
	_port(80)
{
}

Ip::~Ip() {
}

Ip::Ip(const Ip & src) :
	_host(src.getHost()),
	_port(src.getPort())
{
}

Ip & Ip::operator=(const Ip & src) {
	if (this == &src || src.valid() == false)
		return (*this);
	_host = src.getHost();
	_port = src.getPort();
	return (*this);
}

void Ip::setHost(const std::string & host) {
	this->_host = host;
}

const std::string & Ip::getHost() const {
	return (this->_host);
}

void Ip::setPort(unsigned short port) {
	this->_port = port;
}

unsigned short Ip::getPort() const {
	return (this->_port);
}

std::string Ip::str() const {
	std::ostringstream oss;
	oss << this->_host << ":" << this->_port;
	return (oss.str());
}

void Ip::clear() {
	this->_host = "0.0.0.0";
	this->_port = 80;
}

Ip::Ip(const ICopy & src) {
	this->copy(src);
}

Ip & Ip::operator=(const ICopy & src) {
	if (this == &src)
		return (*this);
	this->copy(src);
	return (*this);
}

void Ip::copy(const ICopy & src) {
	const Ip * ip = dynamic_cast<const Ip *>(&src);
	if (!ip || ip->valid() == false)
		return ;
	this->_host = ip->getHost();
	this->_port = ip->getPort();
}

IClone * Ip::clone() const {
	return (new Ip(*this));
}

std::string Ip::_extractHost(const std::string & hp) {
	if (hp.empty())
		return ("");
	size_t cpos = hp.find(':');
	if (cpos == std::string::npos || cpos != hp.rfind(':'))
		return ("");
	return (hp.substr(0, cpos));
}

int Ip::_extractPort(const std::string & hp) {
	if (hp.empty())
		return (-1);
	unsigned int port = 0;
	size_t cpos = hp.find(':');
	if (cpos == std::string::npos || cpos != hp.rfind(':'))
		return (-1);
	std::istringstream pstream(hp.substr(cpos + 1));
	if (!(pstream >> port) || !pstream.eof() || port > 65535)
		return (-1);
	return (static_cast<int>(port));
}

bool Ip::_isValidHost(const std::string & host) const {
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

bool Ip::_isValidPort(int port) const {
	return (port > 0 && port < 65536);
}

void Ip::read(std::istream & is) {
	std::string host = "0.0.0.0";
	int port = 80;
	std::string line;
	std::getline(is, line);

	if (line.empty())
		return;

	// @todo add Utils function to trim string
	size_t start = line.find_first_not_of(" \t\n\r");
	if (start == std::string::npos)
		return;
	size_t end = line.find_last_not_of(" \t\n\r");
	line = line.substr(start, end - start + 1);
	//

	host = this->_extractHost(line);
	if (!(this->_isValidHost(host)))
		return;
	port = this->_extractPort(line);
	if (!(this->_isValidPort(port)))
		return;
	this->_host = host;
	this->_port = static_cast<unsigned short>(port);
}

void Ip::write(std::ostream & os) const {
	os << this->str();
}

bool Ip::operator==(const IEqual & other) const {
	const Ip * ip = dynamic_cast<const Ip *>(&other);
	if (!ip)
		return (false);
	return (this->_host == ip->getHost() && this->_port == ip->getPort());
}

bool Ip::operator!=(const IEqual & other) const {
	return (!(*this == other));
}

static unsigned long u_host(const std::string & ip) {
	char dot;
	unsigned int a, b, c, d;
	std::istringstream iss(ip);
	if (!(iss >> a >> dot >> b >> dot >> c >> dot >> d) ||
		a > 255 || b > 255 || c > 255 || d > 255)
		return (0);
	return ((a << 24) | (b << 16) | (c << 8) | d);
}

bool Ip::operator<(const ICompare & other) const {
	const Ip * ip = dynamic_cast<const Ip *>(&other);
	if (!ip)
		return (false);
	return (u_host(this->_host) < u_host(ip->getHost()));
}

bool Ip::operator<=(const ICompare & other) const {
	const Ip * ip = dynamic_cast<const Ip *>(&other);
	if (!ip)
		return (false);
	return (u_host(this->_host) <= u_host(ip->getHost()));
}

bool Ip::operator>(const ICompare & other) const {
	const Ip * ip = dynamic_cast<const Ip *>(&other);
	if (!ip)
		return (false);
	return (u_host(this->_host) > u_host(ip->getHost()));
}

bool Ip::operator>=(const ICompare & other) const {
	const Ip * ip = dynamic_cast<const Ip *>(&other);
	if (!ip)
		return (false);
	return (u_host(this->_host) >= u_host(ip->getHost()));
}

bool Ip::valid() const {
	return (
		this->_isValidHost(this->_host) &&
		this->_isValidPort(this->_port)
	);
}
