#ifndef UPLOADHANDLER_HPP
#define UPLOADHANDLER_HPP

#include "../webserv.hpp"

namespace Handler
{

	class RequestHandler;

	class UploadHandler: public EventHandler
	{

		private:

			RequestHandler *	_handler;
			std::string	_path;
			const LocationConfig *	_loc;
			IFile	* _file;
			Buffer	_file_buf;

		public:

			UploadHandler(RequestHandler * handler, const std::string & path, const LocationConfig * loc);
			~UploadHandler();

			void handle(short e);
			void onTimeout();
			bool upload(const std::string &file, const std::string &data, Http::Response & res);

		private:

			UploadHandler();
			UploadHandler(const UploadHandler &);
			UploadHandler & operator=(const UploadHandler &);

			void handleMultipart(Http::Request & req, Http::Response & res);
			void handleUrlEncoded(Http::Request & req, Http::Response & res);

	};

} // namespace Handler

#endif // UPLOADHANDLER_HPP