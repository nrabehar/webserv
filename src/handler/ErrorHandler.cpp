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
	{
		custom_page = it->second;
		UriHandler uri_handler(custom_page, loc, _handler);
		custom_page = uri_handler.buildPath();
		if (!uri_handler.fileExists(custom_page) || !uri_handler.isReadable(custom_page))
			custom_page.clear();

		if (!custom_page.empty())
			return (serveCustomPage(custom_page, res));
	}

	loadHtmlErrorPage(res.status(), res);
	_handler->setStatus(HS_OK);

}

void	ErrorHandler::serveCustomPage(const std::string & path, Http::Response & res)
{

	_handler->setStatus(HS_WAITING);
	std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
	if (!file.good())
		return (loadHtmlErrorPage(res.status(), res));
	std::list<std::string> contents;
	char buffer[4096];
	while (file.read(buffer, sizeof(buffer)))
		contents.push_back(std::string(buffer, sizeof(buffer)));
	contents.push_back(std::string(buffer, file.gcount()));
	file.close();
	size_t total_size = 0;
	for (std::list<std::string>::iterator it = contents.begin(); it != contents.end(); ++it)
		total_size += it->size();
	res.setHeader("Content-Length", String::str(total_size));
	std::string headers = res.str();
	total_size += headers.size() + 1;
	try
	{
		size_t offset = 0;
		char * buf = new char[total_size];
		std::memset(buf, 0, total_size);
		std::memmove(buf, headers.c_str(), headers.size());
		offset += headers.size();
		for (std::list<std::string>::iterator it = contents.begin(); it != contents.end(); ++it)
		{
			std::memmove(buf + offset, it->c_str(), it->size());
			offset += it->size();
		}
		_handler->client()->setOut(buf, total_size);
	}
	catch(const std::exception& e)
	{
		(void)e;
		ERR("Error while preparing file content: " + std::string(e.what()));
		return (loadHtmlErrorPage(res.status(), res));
	}
}

void	ErrorHandler::loadHtmlErrorPage(int status, Http::Response & res)
{

	std::string body = UI::getErrorPage(status, res.reason(status));
	res.setHeader("Content-Length", String::str(body.length()));

	size_t total_size = 0;
	std::string headers = res.str();
	total_size += headers.size() + 1;
	total_size += body.size();
	char * buf = new char[total_size];
	std::memset(buf, 0, total_size);
	size_t offset = 0;
	std::memmove(buf, headers.c_str(), headers.size());
	offset += headers.size();
	std::memmove(buf + offset, body.c_str(), body.size());
	offset += body.size();
	_handler->client()->setOut(buf, total_size);
	_handler->setStatus(HS_OK);

}
