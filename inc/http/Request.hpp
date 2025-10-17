#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../webserv.hpp"

namespace Http
{

	struct RequestBody
	{

		std::string field;
		std::string filename;
		std::string value;

	};

	class Request
	{

		private:

			std::string					_method;
			std::string					_uri;
			std::string					_version;
			std::map<std::string, std::string>	_headers;
			std::vector<RequestBody>	_body;
			std::string					_raw_body;

			size_t			_ct_len;
			std::string	_ct_type;

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

			const std::string & body() const;
			const std::vector<RequestBody> & bodyFields() const;
			void appendBody(const std::string & body);
			void addBodyField(const std::string & field, const std::string & filename, const std::string & value);

			size_t	contentLength() const;
			void	setContentLength(size_t cl);

			const std::string & contentType() const;
			void	setContentType(const std::string & ct);

			void	cleanup();

		private:

			Request(const Request &);

	};

}

#endif
