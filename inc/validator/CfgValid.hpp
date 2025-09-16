#ifndef CFGVALID_HPP
#define CFGVALID_HPP

#include "../webserv.hpp"

class CfgValid :
	public ICfgValid
{

	public:

		CfgValid();
		virtual ~CfgValid();

		virtual std::string str() const;

		virtual bool valid() const;

		virtual void visit(const IData &) const;

	private:

		virtual bool _isValidErrorpage(const std::map<int, std::string> &) const;
		virtual bool _isValidMaxBodySize(size_t) const;
		virtual bool _isValidMethod(const std::vector<std::string> &) const;
		virtual bool _isValidRedirect(const std::map<int, std::string> &) const;
		virtual bool _isValidRoot(const std::string &) const;
		virtual bool _isValidIndex(const std::vector<std::string> &) const;
		virtual bool _isValidUploadPath(const std::string &) const;
		virtual bool _isValidCgi(const std::map<std::string, std::string> &) const;

		CfgValid(const CfgValid &);
		CfgValid & operator=(const CfgValid &);

};

#endif
