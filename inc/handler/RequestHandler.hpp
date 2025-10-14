#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "../webserv.hpp"
#include "../http/Request.hpp"
#include "../http/Response.hpp"
#include "ErrorHandler.hpp"

namespace Net
{
	class Client;
}

namespace Handler
{

	class CgiHandler;

	enum Status
	{

		HS_OK = 200,

		HS_BAD_REQUEST = 400,
		HS_FORBIDDEN = 403,
		HS_NOT_FOUND = 404,
		HS_METHOD_NOT_ALLOWED = 405,
		HS_CONFLICT = 409,
		HS_REQUEST_ENTITY_TOO_LARGE = 413,
		HS_URI_TOO_LONG = 414,
		HS_UNSUPPORTED_MEDIA_TYPE = 415,
		HS_UPROCESSABLE_ENTITY = 422,
		HS_INTERNAL_SERVER_ERROR = 500,
		HS_NOT_IMPLEMENTED = 501,
		HS_BAD_GATEWAY = 502,
		HS_SERVICE_UNAVAILABLE = 503,
		HS_GATEWAY_TIMEOUT = 504,

		HS_FOLDER_LISTING = 1000,
		HS_PROGRESS = 1001,
		HS_WAITING = 1002,
		HS_CGI = 1003,
	
	};

	class ErrorHandler;

	class IRequestHandler
	{
	
		public:
	
			virtual ~IRequestHandler() {}
			virtual	void	handle(Http::Request & req, Http::Response & res) = 0;

			virtual	void	addProcess(IEventHandler * h, short e) = 0;
			virtual	void	delProcess(IEventHandler * h) = 0;
	
	};


	class RequestHandler : public IRequestHandler
	{
	
		private:
	
			Net::Client *	_client;
			Status	_status;
			ErrorHandler _error_handler;
			CgiHandler	 * _cgi_handler;

			std::vector<IEventHandler *> _process;

			Http::Request * _req;
			Http::Response * _res;
	
		public:
	
			RequestHandler(Net::Client * client);
			~RequestHandler();

			virtual void	handle(Http::Request & req, Http::Response & res);
			virtual	void	addProcess(IEventHandler * h, short e);
			virtual	void	delProcess(IEventHandler * h);
			Status	status() const;
			
			void	setStatus(Status state);
			void  reset();
			Net::Client * client() const;
			bool isError() const;
			
			void serveError(Status status, const LocationConfig * loc, Http::Response & res);
			void serveError(const LocationConfig * loc, Http::Response & res);

			void serveDirectory(const std::string & path, const std::string & uri, Http::Response & res);

			bool	isCgiRequest(Http::Request & req);
			bool	initCgiHandler(Http::Request & req, Http::Response & res);
			CgiHandler *	cgiHandler();
			const LocationConfig * findLocation(const std::string & uri) const;
	
		private:
	
			RequestHandler();
			RequestHandler(const RequestHandler &);
			RequestHandler & operator=(const RequestHandler &);

			void	mergeHeaders(Http::Request & req, Http::Response & res);
			void  redirect(const std::pair<int, std::string> & redirect, Http::Response & res);
	
	};
	

} // namespace Handler

#include "UriHandler.hpp"
#include "StaticHandler.hpp"
#include "UploadHandler.hpp"
#include "CgiHandler.hpp"

#include "MethodHandler.hpp"

#endif // REQUESTHANDLER_HPP