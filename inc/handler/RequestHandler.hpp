#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "../webserv.hpp"
#include "../http/Request.hpp"
#include "../http/Response.hpp"

namespace Net
{
	class Client;
}

namespace Handler
{

	enum State
	{

		HS_OK = 200,

		HS_FORBIDDEN = 403,
		HS_NOT_FOUND = 404,

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
	
	};
	

	class RequestHandler : public IRequestHandler
	{
	
		private:
	
			Net::Client *	_client;
			State	_state;
	
		public:
	
			RequestHandler(Net::Client * client);
			~RequestHandler();

			virtual void	handle(Http::Request & req, Http::Response & res);
			State	state() const;
			
			void	setState(State state);
			void  reset();
			Net::Client * client() const;
	
		private:
	
			RequestHandler();
			RequestHandler(const RequestHandler &);
			RequestHandler & operator=(const RequestHandler &);

			const LocationConfig * findLocation(const std::string & uri) const;
			void	mergeHeaders(Http::Request & req, Http::Response & res);
	
	};
	

} // namespace Handler

#include "UriHandler.hpp"
#include "ErrorHandler.hpp"

#endif // REQUESTHANDLER_HPP