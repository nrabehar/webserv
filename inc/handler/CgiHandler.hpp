#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../webserv.hpp"
#include "../http/Request.hpp"
#include "../http/Response.hpp"
#include "../core/EventHandler.hpp"

namespace Handler
{

class RequestHandler;

class CgiHandler
{

	private:

		pid_t	_pid;
		RequestHandler * _handler;
		Http::Request * _req;
		Http::Response * _res;
		std::map<std::string, std::string> _env;
		std::vector<const char *> _arg;
		Buffer _in;
		Buffer _out;
		class CgiStdinHandler;
		class CgiStdoutHandler;

		CgiStdinHandler * _stdin_h;
		CgiStdoutHandler * _stdout_h;
		int _timeout;

	public:


		CgiHandler(RequestHandler * handler, Http::Request * req, Http::Response * res, int timeout = 30);
		~CgiHandler();

		void closeIn(Status st = HS_OK);
		void closeOut(Status st = HS_PROGRESS);
		void write(const char *data, size_t len);
		void launch(const std::string & bin, const std::string & script);

	private:

		CgiHandler(const CgiHandler &);
		CgiHandler & operator=(const CgiHandler &);
		void	initEnv();
		const char **	getArg(const std::string & bin, const std::string & script);
		char **	mapToCArray(const std::map<std::string, std::string> & m) const;
		void	freeCArray(char ** arr) const;
		std::string headerKeyToEnv(const std::string & key) const;

		void processOutput();
		bool parseHeaders();
		bool parseBody();

		class CgiStdinHandler : public EventHandler
		{

			private:
				
				CgiHandler * _cgi;
				Buffer * _in;
				size_t _offset;

			public:

				CgiStdinHandler(CgiHandler * cgi, Buffer * in, int fd);
				~CgiStdinHandler();
				virtual void handle(short e);
				virtual void onTimeout();

			private:

				CgiStdinHandler();
				CgiStdinHandler(const CgiStdinHandler &);
				CgiStdinHandler & operator=(const CgiStdinHandler &);

		};

		class CgiStdoutHandler : public EventHandler
		{
			
			private:

				CgiHandler * _cgi;
				Buffer * _out;
				size_t _offset;

			public:
		
				CgiStdoutHandler(CgiHandler * cgi, Buffer * out, int fd);
				~CgiStdoutHandler();
				virtual void handle(short e);
				virtual void onTimeout();

			private:

				CgiStdoutHandler();
				CgiStdoutHandler(const CgiStdoutHandler &);
				CgiStdoutHandler & operator=(const CgiStdoutHandler &);

		};

};


}	

#endif