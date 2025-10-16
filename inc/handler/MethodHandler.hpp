#ifndef METHODHANDLER_HPP
#define METHODHANDLER_HPP

#include "../webserv.hpp"

namespace Handler
{

	class RequestHandler;
	
	class MethodHandler
	{
	
		private:
	
			RequestHandler * _handler;
			std::string _path;
			const LocationConfig * _loc;
			
		public:
	
			MethodHandler(RequestHandler * handler, const std::string & path, const LocationConfig * loc);
			~MethodHandler();
			void handle(Http::Request & req, Http::Response & res);
	
		private:
	
			MethodHandler();
			MethodHandler(const MethodHandler &);
			MethodHandler & operator=(const MethodHandler &);

			void handleGet(Http::Request & req, Http::Response & res);
			void handlePost(Http::Request & req, Http::Response & res);
			void handleDelete(Http::Request & req, Http::Response & res);

			void createJsonResponse(const std::vector<std::pair<std::string, std::string> > & files,
				const std::vector<std::pair<std::string, std::string> > & fields, Http::Response & res);
	
	};
	

} 


#endif