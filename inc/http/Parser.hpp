#ifndef PARSER_HPP
#define PARSER_HPP

#include "../webserv.hpp"
#include "../core/Buffer.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace Handler
{
	class RequestHandler;
}
namespace Http
{
	
	class Parser
	{
	
		private:
	
			enum Stage
			{
				REQUEST_LINE,
				HEADERS,
				BODY,
				DONE,
				ERROR
			};

			enum MultipartState
			{
				BS_BOUNDARY,
				BS_HEADER,
				BS_PART,
				BS_DONE,
				BS_ERROR
			};

			enum BodyType
			{
				MULTIPART,
				URLENCODED,
				CHUNKED,
				RAW,
			};

			Stage	_stage;
			MultipartState	_mp_state;
			BodyType	_body_type;

			std::string	_field;
			std::string _filename;
			std::string _value;
			std::string _tmp_filename;

			IFile	* _tmp_file;
			std::string	_boundary;
			Handler::RequestHandler * _req_handler;
			bool	_is_cgi;
			
		public:
	
			Parser(Handler::RequestHandler * req_handler);
			~Parser();

			enum ParseState
			{
				PS_DONE,
				PS_WAIT,
				PS_INCOMPLETE,
				PS_ERROR
			} _ps_state;

			bool parseNext(Buffer & buf, Request & req, Response & res);

			ParseState state() const;
			void reset();
	
		private:

			Parser(const Parser &);
			Parser & operator=(const Parser &);

			void setStage(Stage stage);

			bool parseReqLine(Buffer & buf, Request & req);
			bool parseHeaders(Buffer & buf, Request & req, Response & res);
			bool parseBody(Buffer & buf, Request & req);

			bool parseUrlEncoded(Buffer & buf, Request & req);
			bool parseMultiPartBody(Buffer & buf, Request & req);
			bool parseChunkedBody(Buffer & buf, Request & req);

			std::string getBoundary(const std::string & ct);
			void parseField(const std::string & part, Request & req);
			std::string parseDisposition(const std::string & disp, const std::string & field);

			std::string parsePercentEncoding(const std::string & str);

			bool bodyMultipartBoundary(Buffer & buf, Request & req);
			bool bodyMultipartHeader(Buffer & buf, Request & req);
			bool bodyMultipartPart(Buffer & buf, Request & req);

			bool waitForRecycle(Buffer & buf, Request & req);

	};
	

} 


#endif