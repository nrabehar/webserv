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

		// Setters/Getters
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

		// IData
		virtual std::string str() const;

		// IClear
		virtual void clear();

		// ICopy
		Cfg(const ICopy &);
		Cfg & operator=(const ICopy &);
		virtual void copy(const ICopy &);

		// IClone
		virtual IClone * clone() const;

		// IIO
		virtual void read(std::istream &);
		virtual void write(std::ostream &) const;

		// IEqual
		virtual bool operator==(const IEqual &) const;
		virtual bool operator!=(const IEqual &) const;

		// IValid
		virtual bool valid() const;

	protected:

		virtual bool _isValidErrorpage(const std::map<int, std::string> &) const;
		virtual bool _isValidMaxBodySize(size_t) const;
		virtual bool _isValidMethod(const std::vector<std::string> &) const;
		virtual bool _isValidRedirect(const std::map<int, std::string> &) const;
		virtual bool _isValidRoot(const std::string &) const;
		virtual bool _isValidIndex(const std::vector<std::string> &) const;
		virtual bool _isValidUploadPath(const std::string &) const;
		virtual bool _isValidCgi(const std::map<std::string, std::string> &) const;

};

#endif // CFG_HPP
