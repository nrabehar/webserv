#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../webserv.hpp"

namespace Http
{
	
	class Response
	{
	
		private:
	
			int									_status;
			std::string					_version;
			std::string					_reason;
			std::map<std::string, std::string>	_headers;
			std::string					_body;
				
		public:
	
			Response();
			Response(int status, const std::string & reason);
			Response & operator=(const Response &);
			~Response();

			const std::string & version() const;
			void setVersion(const std::string & version);

			int statusCode() const;
			void setStatusCode(int status);

			const std::map<std::string, std::string> & header() const;
			const std::string & header(const std::string & key) const;
			void setHeader(const std::string & key, const std::string & value);
			const std::string & body() const;
			void appendBody(const std::string & body);
			const std::string & reason() const;
			void setReason(const std::string & reason);

			std::string str() const;
	
		private:
	
			Response(const Response &);
	
	};
	

} // namespace Http

#endif // RESPONSE_HPP