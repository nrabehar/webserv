#ifndef CFG_IPP
#define CFG_IPP 1

#include "../webserv.hpp"

class ICfg :
	public virtual IClear,
	public virtual ICopy,
	public virtual IClone,
	public virtual IIO,
	public virtual IEqual,
	public virtual IValid
{
	public:
		virtual ~ICfg() {};
		virtual void addErrorpage(int, const std::string &) = 0;
		virtual const std::map<int, std::string> & getErrorpage() const = 0;
		virtual void setMaxBodySize(size_t) = 0;
		virtual const size_t & getMaxBodySize() const = 0;
		virtual void addMethod(const std::string &) = 0;
		virtual const std::vector<std::string> & getMethod() const = 0;
		virtual void addRedirect(int, const std::string &) = 0;
		virtual const std::map<int, std::string> & getRedirect() const = 0;
		virtual void setRoot(const std::string &) = 0;
		virtual const std::string & getRoot() const = 0;
		virtual void setAutoindex(bool) = 0;
		virtual const bool & getAutoindex() const = 0;
		virtual void addIndex(const std::string &) = 0;
		virtual const std::vector<std::string> & getIndex() const = 0;
		virtual void setUploadPath(const std::string &) = 0;
		virtual const std::string & getUploadPath() const = 0;
		virtual void addCgi(const std::string &, const std::string &) = 0;
		virtual const std::map<std::string, std::string> & getCgi() const = 0;
};

#endif // CFG_IPP
