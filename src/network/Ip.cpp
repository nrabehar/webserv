#include "webserv.hpp"

Ip::Ip() :
	_host(DEFAULT_SERVER_HOST),
	_port(DEFAULT_SERVER_PORT)
{
}

Ip::~Ip() {
	this->clear();
}

Ip::Ip(const Ip & src) :
	_host(DEFAULT_SERVER_HOST),
	_port(DEFAULT_SERVER_PORT)
{
	(*this) = src;
}

Ip & Ip::operator=(const Ip & src) {
	if (this == &src || src.valid() == false)
		return (*this);
	this->_host = src.getHost();
	this->_port = src.getPort();
	return (*this);
}

void Ip::setHost(const std::string & host) {
	this->_host = host;
}

const std::string & Ip::getHost() const {
	return (this->_host);
}

void Ip::setPort(int port) {
	this->_port = port;
}

int Ip::getPort() const {
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

Ip::Ip(const ICopy & src) :
	_host(DEFAULT_SERVER_HOST),
	_port(DEFAULT_SERVER_PORT)
{
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

void Ip::read(std::istream & is) {
	IpParse v(is);
	this->accept(v);
	this->_host = v.getHost();
	this->_port = v.getPort();
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
	IpValid v;
	this->accept(v);
	return (v.valid());
}

void Ip::accept(const IGuest & g) const {
	g.visit(*this);
}
