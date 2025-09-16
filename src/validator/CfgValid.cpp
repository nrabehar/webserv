#include "webserv.hpp"

CfgValid::CfgValid(const IData * data) :
	_data(data)
{
}

CfgValid::~CfgValid() {
}

std::string CfgValid::str() const {
	const Cfg * c = dynamic_cast<const Cfg *>(this->_data);
	if (!c)
		return ("");
	return (c->str());
}

bool CfgValid::valid() const {
	const Cfg * c = dynamic_cast<const Cfg *>(this->_data);
	if (!c)
		return (false);
	return (
		this->_isValidErrorpage(c->getErrorpage()) &&
		this->_isValidMaxBodySize(c->getMaxBodySize()) &&
		this->_isValidMethod(c->getMethod()) &&
		this->_isValidRedirect(c->getRedirect()) &&
		this->_isValidRoot(c->getRoot()) &&
		this->_isValidIndex(c->getIndex()) &&
		this->_isValidUploadPath(c->getUploadPath()) &&
		this->_isValidCgi(c->getCgi())
	);
}

bool CfgValid::_isValidErrorpage(const std::map<int, std::string> & e) const {
	for (std::map<int, std::string>::const_iterator it = e.begin(); it != e.end(); ++it) {
		// @audit Check for path validity (non-empty, absolute path, no spaces, etc.)
		if (it->first < 400 || it->first > 599 || it->second.empty())
			return (false);
	}
	return (true);
}

bool CfgValid::_isValidMaxBodySize(size_t s) const {
	// @todo Implement validation logic
	return (true);
}

bool CfgValid::_isValidMethod(const std::vector<std::string> & m) const {
	// @todo Implement validation logic
	(void)m;
	return (true);
}

bool CfgValid::_isValidRedirect(const std::map<int, std::string> & r) const {
	// @todo Implement validation logic
	(void)r;
	return (true);
}

bool CfgValid::_isValidRoot(const std::string & r) const {
	// @todo Implement validation logic
	(void)r;
	return (true);
}

bool CfgValid::_isValidIndex(const std::vector<std::string> & i) const {
	// @todo Implement validation logic
	(void)i;
	return (true);
}

bool CfgValid::_isValidUploadPath(const std::string & u) const {
	// @todo Implement validation logic
	(void)u;
	return (true);
}

bool CfgValid::_isValidCgi(const std::map<std::string, std::string> & c) const {
	// @todo Implement validation logic
	(void)c;
	return (true);
}
