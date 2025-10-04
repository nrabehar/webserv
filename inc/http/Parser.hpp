#ifndef PARSER_HPP
#define PARSER_HPP

#include "../webserv.hpp"
#include "../core/Buffer.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace Http
{
	
	class Parser
	{
	
		private:
	
			enum State
			{
				REQUEST_LINE,
				HEADERS,
				BODY,
				DONE,
				ERROR
			};	

			State	_state;
						
		public:
	
			Parser();
			~Parser();

			enum ParseState
			{
				PS_DONE,
				PS_INCOMPLETE,
				PS_ERROR
			};

			bool parseNext(Buffer & buf, Request & req);

			ParseState state() const;
			void reset();
	
		private:

			ParseState	_usr_state;
	
			Parser(const Parser &);
			Parser & operator=(const Parser &);

			void setState(State state);

			bool parseReqLine(Buffer & buf, Request & req);
			bool parseHeaders(Buffer & buf, Request & req);
			bool parseBody(Buffer & buf, Request & req);

			bool parseUrlEncoded(Request & req);
			bool parseMultiPartBody(Request & req);
			bool parseChunkedBody(Buffer & buf, Request & req);

			std::string getBoundary(const std::string & ct);
			void parseField(const std::string & part, Request & req);
			std::string parseDisposition(const std::string & disp, const std::string & field);

			std::string parsePercentEncoding(const std::string & str);


	};
	

} // namespace Http


#endif // PARSER_HPP