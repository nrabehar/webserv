#ifndef STATICHANDLER_HPP
#define STATICHANDLER_HPP

#include "../webserv.hpp"

namespace Handler
{
	
	class StaticHandler : public EventHandler
	{
	
		private:

			std::string	_path;
			size_t		_offset;
			IFile *	_file;
			RequestHandler *	_handler;
			Http::Response *	_response;
			const LocationConfig *	_loc;
	
		public:
	
			StaticHandler(RequestHandler * handler);
			virtual ~StaticHandler();
			virtual void	handle(short e);
			virtual void	onTimeout();
			bool handle(const std::string & path, Http::Response * res);
	
		private:
	
			StaticHandler(const StaticHandler &);
			StaticHandler & operator=(const StaticHandler &);
	
	};
	

} // namespace Handler


#endif // STATICHANDLER_HPP