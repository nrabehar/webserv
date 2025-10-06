#ifndef URIHANDLER_HPP
#define URIHANDLER_HPP

#include "../webserv.hpp"

namespace Handler
{
	
	class UriHandler
	{
	
		private:
	
			std::string	_uri;
			const LocationConfig *	_loc;
			RequestHandler *	_handler;
	
		public:
	
			UriHandler(const std::string & uri, const LocationConfig * loc, RequestHandler * handler);
			~UriHandler();

			const std::string & uri() const;

			std::string buildPath();
			static std::string buildPath(const std::string & uri, const LocationConfig * loc);
			bool fileExists(const std::string & path) const;
			bool isDirectory(const std::string & path) const;
			bool isReadable(const std::string & path) const;
	
		private:
	
			UriHandler(const UriHandler &);
			UriHandler & operator=(const UriHandler &);
			void getIndexPath(std::string & path);
			bool isCgiPath(const std::string & path) const;
	
	};
	

} // namespace Handler


#endif // URIHANDLER_HPP