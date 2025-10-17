#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "../webserv.hpp"
#include "../http/Request.hpp"
#include "../http/Response.hpp"
#include "ErrorHandler.hpp"
#include "HandlerStatus.hpp"
#include "CgiHandler.hpp"

namespace Net
{
	class Client;
}

namespace Handler
{

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
			CgiHandler 	 _cgi_handler;

			std::vector<IEventHandler *> _process;

			Http::Request * _req;
			Http::Response * _res;

		public:

			RequestHandler(Net::Client * client, Http::Request * req, Http::Response * res);
			~RequestHandler();

			virtual void	handle(Http::Request & req, Http::Response & res);
			virtual	void	addProcess(IEventHandler * h, short e);
			virtual	void	delProcess(IEventHandler * h);
			Status	status() const;
			void	notifyTimeout();

			void	setStatus(Status state);
			void  reset();
			Net::Client * client() const;
			Http::Response * response();
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

}

#include "UriHandler.hpp"
#include "StaticHandler.hpp"
#include "CgiHandler.hpp"

#include "MethodHandler.hpp"

#endif
