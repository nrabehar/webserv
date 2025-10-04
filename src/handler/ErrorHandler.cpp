#include "webserv.hpp"

using namespace Handler;

ErrorHandler::ErrorHandler(RequestHandler * handler)
	: _handler(handler) {}
ErrorHandler::~ErrorHandler() {}

void	ErrorHandler::handle(const  LocationConfig * loc, Http::Response & res)
{
	
	res.setStatus((int)_handler->status());
	res.setHeader("Content-Type", "text/html; charset=UTF-8");

	std::string custom_page;
	std::map<int, std::string>::const_iterator it = loc->err_page.find(res.status());
	if (it != loc->err_page.end())
		custom_page = it->second;

	UriHandler uri_handler(custom_page, loc, _handler);
	if (!uri_handler.fileExists(custom_page) || !uri_handler.isReadable(custom_page))
		custom_page.clear();

	if (!custom_page.empty())
		return (serveCustomPage(custom_page, res));

	loadHtmlErrorPage(res.status(), res);
	_handler->setStatus(HS_OK);

}

void	ErrorHandler::serveCustomPage(const std::string & path, Http::Response & res)
{

	(void)path;
	// TODO create static file server (EventHandler) to serve the file
	// TODO serve the file at path
	// TODO set state to waiting and return
	_handler->setStatus(HS_WAITING);
	StaticHandler * static_handler = new StaticHandler(_handler);
	if (!static_handler->handle(path, &res))
	{
		delete static_handler;
		return (loadHtmlErrorPage(_handler->status(), res));
	}
	EventLoop::instance().addHandler(static_handler, POLLIN);
}

void	ErrorHandler::loadHtmlErrorPage(int status, Http::Response & res)
{

	std::string body = "<!DOCTYPE html>";
	body += "<html><head><title>";
	body += String::str(status) + " " + res.reason() + "</title></head>";
	body += "<body><h1>" + String::str(status) + " " + res.reason() + "</h1>";
	body += "<hr><em>webserv</em></body></html>";

	res.appendBody(body);
	res.setHeader("Content-Length", String::str(res.body().length()));
	_handler->setStatus(HS_OK);

}
