#include "webserv.hpp"

Cfg::Cfg() :
	_errorpage(),
	_maxBodySize(DEFAULT_MAX_BODY_SIZE),
	_method(),
	_redirect(),
	_root(DEFAULT_SERVER_ROOT),
	_autoindex(false),
	_index(),
	_uploadPath(DEFAULT_UPLOAD_PATH),
	_cgi()
{
}

Cfg::~Cfg() {
	this->clear();
}

Cfg::Cfg(const Cfg & src) :
	_errorpage(),
	_maxBodySize(DEFAULT_MAX_BODY_SIZE),
	_method(),
	_redirect(),
	_root(DEFAULT_SERVER_ROOT),
	_autoindex(false),
	_index(),
	_uploadPath(DEFAULT_UPLOAD_PATH),
	_cgi()
{
	(*this) = src;
}

Cfg & Cfg::operator=(const Cfg & src) {
	if (this == &src || src.valid() == false)
		return (*this);
	this->_errorpage = src.getErrorpage();
	this->_maxBodySize = src.getMaxBodySize();
	this->_method = src.getMethod();
	this->_redirect = src.getRedirect();
	this->_root = src.getRoot();
	this->_autoindex = src.getAutoindex();
	this->_index = src.getIndex();
	this->_uploadPath = src.getUploadPath();
	this->_cgi = src.getCgi();
	return (*this);
}

// Setters/Getters

void Cfg::addErrorpage(int code, const std::string & path) {
	this->_errorpage[code] = path;
}

const std::map<int, std::string> & Cfg::getErrorpage() const {
	return (this->_errorpage);
}

void Cfg::setMaxBodySize(size_t size) {
	this->_maxBodySize = size;
}

const size_t & Cfg::getMaxBodySize() const {
	return (this->_maxBodySize);
}

void Cfg::addMethod(const std::string & method) {
	if (std::find(this->_method.begin(), this->_method.end(), method) == this->_method.end())
		this->_method.push_back(method);
}

const std::vector<std::string> & Cfg::getMethod() const {
	return (this->_method);
}

void Cfg::addRedirect(int code, const std::string & url) {
	this->_redirect[code] = url;
}

const std::map<int, std::string> & Cfg::getRedirect() const {
	return (this->_redirect);
}

void Cfg::setRoot(const std::string & root) {
	this->_root = root;
}

const std::string & Cfg::getRoot() const {
	return (this->_root);
}

void Cfg::setAutoindex(bool autoindex) {
	this->_autoindex = autoindex;
}

const bool & Cfg::getAutoindex() const {
	return (this->_autoindex);
}

void Cfg::addIndex(const std::string & index) {
	if (std::find(this->_index.begin(), this->_index.end(), index) == this->_index.end())
		this->_index.push_back(index);
}

const std::vector<std::string> & Cfg::getIndex() const {
	return (this->_index);
}

void Cfg::setUploadPath(const std::string & path) {
	this->_uploadPath = path;
}

const std::string & Cfg::getUploadPath() const {
	return (this->_uploadPath);
}

void Cfg::addCgi(const std::string & ext, const std::string & path) {
	this->_cgi[ext] = path;
}

const std::map<std::string, std::string> & Cfg::getCgi() const {
	return (this->_cgi);
}

// IData

std::string Cfg::str() const {
	std::ostringstream oss;
	oss << "Cfg(";
	oss << "errorpage: {";
	std::vector<std::string>::const_iterator it_vs;
	std::map<int, std::string>::const_iterator it_is;
	std::map<std::string, std::string>::const_iterator it_ss;
	for (it_is = this->_errorpage.begin(); it_is != this->_errorpage.end(); ++it_is) {
		if (it_is != this->_errorpage.begin())
			oss << ", ";
		oss << it_is->first << ": " << it_is->second;
	}
	oss << "}, ";
	oss << "maxBodySize: " << this->_maxBodySize << ", ";
	oss << "method: [";
	for (it_vs = this->_method.begin(); it_vs != this->_method.end(); ++it_vs) {
		if (it_vs != this->_method.begin())
			oss << ", ";
		oss << *it_vs;
	}
	oss << "], ";
	oss << "redirect: {";
	for (it_is = this->_redirect.begin(); it_is != this->_redirect.end(); ++it_is) {
		if (it_is != this->_redirect.begin())
			oss << ", ";
		oss << it_is->first << ": " << it_is->second;
	}
	oss << "}, ";
	oss << "root: " << this->_root << ", ";
	oss << "autoindex: " << (this->_autoindex ? "true" : "false") << ", ";
	oss << "index: [";
	for (it_vs = this->_index.begin(); it_vs != this->_index.end(); ++it_vs) {
		if (it_vs != this->_index.begin())
			oss << ", ";
		oss << *it_vs;
	}
	oss << "], ";
	oss << "uploadPath: " << this->_uploadPath << ", ";
	oss << "cgi: {";
	for (it_ss = this->_cgi.begin(); it_ss != this->_cgi.end(); ++it_ss) {
		if (it_ss != this->_cgi.begin())
			oss << ", ";
		oss << it_ss->first << ": " << it_ss->second;
	}
	oss << "}";
	oss << ")";
	return (oss.str());
}

// IClear

void Cfg::clear() {
	this->_errorpage.clear();
	this->_maxBodySize = DEFAULT_MAX_BODY_SIZE;
	this->_method.clear();
	this->_redirect.clear();
	this->_root.clear();
	this->_root = "/www/";
	this->_autoindex = false;
	this->_index.clear();
	this->_uploadPath.clear();
	this->_uploadPath = "/uploads/";
	this->_cgi.clear();
}

// ICopy

Cfg::Cfg(const ICopy & other) :
	_errorpage(),
	_maxBodySize(DEFAULT_MAX_BODY_SIZE),
	_method(),
	_redirect(),
	_root(DEFAULT_SERVER_ROOT),
	_autoindex(false),
	_index(),
	_uploadPath(DEFAULT_UPLOAD_PATH),
	_cgi()
{
	this->copy(other);
}

Cfg & Cfg::operator=(const ICopy & other) {
	if (this != &other)
		this->copy(other);
	return (*this);
}

void Cfg::copy(const ICopy & other) {
	const Cfg * o = dynamic_cast<const Cfg *>(&other);
	if (!o || o->valid() == false)
		return ;
	this->_errorpage = o->getErrorpage();
	this->_maxBodySize = o->getMaxBodySize();
	this->_method = o->getMethod();
	this->_redirect = o->getRedirect();
	this->_root = o->getRoot();
	this->_autoindex = o->getAutoindex();
	this->_index = o->getIndex();
	this->_uploadPath = o->getUploadPath();
	this->_cgi = o->getCgi();
}

// IClone

IClone * Cfg::clone() const {
	return (new Cfg(*this));
}

// IIO

void Cfg::read(std::istream & i) {
	(void)i;
	// @todo implement
	// Delegate to CfgParse later
}

void Cfg::write(std::ostream & o) const {
	o << this->str();
}

// IEqual

bool Cfg::operator==(const IEqual & other) const {
	const Cfg * o = dynamic_cast<const Cfg *>(&other);
	if (!o)
		return (false);
	// @todo implement
	(void)o;
	return (true);
}

bool Cfg::operator!=(const IEqual & other) const {
	return (!((*this) == other));
}

// IValid

bool Cfg::valid() const {
	return (
		this->_isValidErrorpage(this->_errorpage) &&
		this->_isValidMaxBodySize(this->_maxBodySize) &&
		this->_isValidMethod(this->_method) &&
		this->_isValidRedirect(this->_redirect) &&
		this->_isValidRoot(this->_root) &&
		this->_isValidIndex(this->_index) &&
		this->_isValidUploadPath(this->_uploadPath) &&
		this->_isValidCgi(this->_cgi)
	);
}

// Protected

bool Cfg::_isValidErrorpage(const std::map<int, std::string> & e) const {
	// @todo delegate to CfgValid
	// For future use
	(void)e;
	return (true);
}

bool Cfg::_isValidMaxBodySize(size_t s) const {
	// @todo delegate to CfgValid
	// For future use
	(void)s;
	return (true);
}

bool Cfg::_isValidMethod(const std::vector<std::string> & m) const {
	// @todo delegate to CfgValid
	// For future use
	(void)m;
	return (true);
}

bool Cfg::_isValidRedirect(const std::map<int, std::string> & r) const {
	// @todo delegate to CfgValid
	// For future use
	(void)r;
	return (true);
}

bool Cfg::_isValidRoot(const std::string & r) const {
	// @todo delegate to CfgValid
	// For future use
	(void)r;
	return (true);
}

bool Cfg::_isValidIndex(const std::vector<std::string> & i) const {
	// @todo delegate to CfgValid
	// For future use
	(void)i;
	return (true);
}

bool Cfg::_isValidUploadPath(const std::string & u) const {
	// @todo delegate to CfgValid
	// For future use
	(void)u;
	return (true);
}

bool Cfg::_isValidCgi(const std::map<std::string, std::string> & c) const {
	// @todo delegate to CfgValid
	// For future use
	(void)c;
	return (true);
}
