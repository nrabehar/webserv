#ifndef	HTTP_REQUEST_HPP
#define	HTTP_REQUEST_HPP

#include <iostream>
#include <map>

class	HttpRequest
{

	private:

		std::string _method;
		std::string	_uri;
		std::string	_version;
		std::string	_body;
		std::map<std::string, std::string> _headers;

	public:

		HttpRequest();
		~HttpRequest();

		const std::string&	getMethod() const ;
		const std::string&	getUri() const ;
		const std::string&	getVersion() const ;
		const std::string&	getBody() const;
		const	std::string&	getHeader(const std::string &) const;

	private:

		HttpRequest(const HttpRequest &);
		HttpRequest& operator=(const HttpRequest &);

};

#endif
