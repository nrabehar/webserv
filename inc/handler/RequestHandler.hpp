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

	enum HandlerState
	{
		HS_DONE,
		HS_PROGRESS
	};

	class RequestHandler
	{
	
		protected:
	
			Net::Client *	_client;
			HandlerState	_state;
	
		public:
	
			RequestHandler(Net::Client * client);
			~RequestHandler();

			void	handle(Http::Request & req, Http::Response & res);
			HandlerState	state() const;
			void  reset();

	
		private:
	
			RequestHandler();
			RequestHandler(const RequestHandler &);
			RequestHandler & operator=(const RequestHandler &);

			const LocationConfig * findLocation(const std::string & uri) const;
			void	mergeHeaders(Http::Request & req, Http::Response & res);

		protected:
			
			void	setState(HandlerState state);
	
	};
	

} // namespace Handler

#endif // REQUESTHANDLER_HPP