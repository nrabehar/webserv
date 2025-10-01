#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../webserv.hpp"

namespace Http
{
	
	class Response
	{
	
		private:
	
			std::string					_version;
			int							_status;
			std::map<std::string, std::string>	_headers;
			std::string					_body;
				
		public:
	
			Response();
			Response(int status, const std::string & body);
			~Response();

			const std::string & version() const;
			void setVersion(const std::string & version);

			int statusCode() const;
			void setStatusCode(int status);

			const std::map<std::string, std::string> & header() const;
			const std::string & header(const std::string & key) const;
			void setHeader(const std::string & key, const std::string & value);
	
		private:
	
			Response(const Response &);
			Response & operator=(const Response &);
	
	};
	

} // namespace Http

#endif // RESPONSE_HPP