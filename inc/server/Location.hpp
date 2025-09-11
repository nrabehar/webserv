#ifndef LOCATION_HPP
#define LOCATION_HPP 1

#include "../webserv.hpp"

struct CgiLink
{
	std::string extension;
	std::string path;
};

class Location: public IParser, public IChecker, public IError
{

	private:

		bool _autoindex;
		std::string _uri;
		std::string _root;
		size_t _max_body_size;
		std::string _upload_path;
		std::vector<std::string> _index;
		std::vector<std::string> _method;
		std::map<EStatusCode, std::string> _errorpage;
		std::map<EStatusCode, std::string> _redirect;
		std::vector<CgiLink> _cgi;

	public:

		Location();
		~Location();
		Location(const Location &);
		Location & operator=(const Location &);

		bool getAutoindex() const;
		size_t getMaxBodySize() const;
		const std::string & getUri() const;
		const std::string & getRoot() const;
		const std::string & getUploadPath() const;
		const std::vector<std::string> & getIndex() const;
		const std::vector<std::string> & getMethod() const;
		const std::map<EStatusCode, std::string> & getErrorPage() const;
		const std::map<EStatusCode, std::string> & getRedirect() const;
		const std::vector<CgiLink> & getCgi() const;

		void parse(const std::string &);
		EErrorCode check() const __attribute__((warn_unused_result));
		void reportError(EErrorCode);

		void setAutoindex(bool);
		void setMaxBodySize(size_t);
		void setRoot(const std::string &);
		void setIndex(const std::vector<std::string> &);
		void setErrorPage(const std::map<EStatusCode, std::string> &);

	private:

		void setDirective(const std::string &, const std::string &);

};

#endif // LOCATION_HPP
