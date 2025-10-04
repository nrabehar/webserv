#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include "../webserv.hpp"

namespace Handler
{

	class RequestHandler;

	class ErrorHandler
	{

		private:
			
			RequestHandler *	_handler;
	
		public:
	
			ErrorHandler(RequestHandler * handler);
			~ErrorHandler();
			void	handle(const  LocationConfig * loc, Http::Response & res);
	
		private:
	
			ErrorHandler();
			ErrorHandler(const ErrorHandler &);
			ErrorHandler & operator=(const ErrorHandler &);

			void serveCustomPage(const std::string & path, Http::Response & res);
			void loadHtmlErrorPage(int status, Http::Response & res);
	
	};
	

} // namespace Handler

#endif // ERRORHANDLER_HPP