#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../webserv.hpp"

namespace Http
{
	
	class Request
	{
	
		private:
	
			std::string					_method;
			std::string					_uri;
			std::string					_version;
			std::map<std::string, std::string>	_headers;
			std::string					_body;
				
		public:
	
			Request();
			Request & operator=(const Request &);
			~Request();

			const std::string & method() const;
			void setMethod(const std::string & method);

			const std::string & uri() const;
			void setUri(const std::string & uri);

			const std::string & version() const;
			void setVersion(const std::string & version);

			const std::map<std::string, std::string> & header() const;
			const std::string & header(const std::string & key) const;
			void setHeader(const std::string & key, const std::string & value);
	
		private:
	
			Request(const Request &);
	
	};
	

} // namespace Http


#endif // REQUEST_HPP