#ifndef CFG_HPP
#define CFG_HPP 1

#include "../webserv.hpp"

class Cfg :
	public virtual ICfg
{

	private:
		std::map<int, std::string>					_errorpage;
		size_t															_maxBodySize;
		std::vector<std::string>						_method;
		std::map<int, std::string>					_redirect;
		std::string													_root;
		bool																_autoindex;
		std::vector<std::string>						_index;
		std::string													_uploadPath;
		std::map<std::string, std::string>	_cgi;

	public:

		Cfg();
		~Cfg();
		Cfg(const Cfg &);
		Cfg & operator=(const Cfg &);

		virtual void addErrorpage(int, const std::string &);
		virtual const std::map<int, std::string> & getErrorpage() const;
		virtual void setMaxBodySize(size_t);
		virtual const size_t & getMaxBodySize() const;
		virtual void addMethod(const std::string &);
		virtual const std::vector<std::string> & getMethod() const;
		virtual void addRedirect(int, const std::string &);
		virtual const std::map<int, std::string> & getRedirect() const;
		virtual void setRoot(const std::string &);
		virtual const std::string & getRoot() const;
		virtual void setAutoindex(bool);
		virtual const bool & getAutoindex() const;
		virtual void addIndex(const std::string &);
		virtual const std::vector<std::string> & getIndex() const;
		virtual void setUploadPath(const std::string &);
		virtual const std::string & getUploadPath() const;
		virtual void addCgi(const std::string &, const std::string &);
		virtual const std::map<std::string, std::string> & getCgi() const;

		virtual std::string str() const;

		virtual void clear();

		Cfg(const ICopy &);
		Cfg & operator=(const ICopy &);
		virtual void copy(const ICopy &);

		virtual IClone * clone() const;

		virtual void read(std::istream &);
		virtual void write(std::ostream &) const;

		virtual bool operator==(const IEqual &) const;
		virtual bool operator!=(const IEqual &) const;

		virtual bool valid() const;

		virtual void accept(const IGuest &) const;

};

#endif
