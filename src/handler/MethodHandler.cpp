#include "webserv.hpp"

using namespace Handler;

MethodHandler::MethodHandler(RequestHandler * handler, const std::string & path, const LocationConfig * loc)
	: _handler(handler), _path(path), _loc(loc) {}

MethodHandler::~MethodHandler() {}

void MethodHandler::handle(Http::Request & req, Http::Response & res)
{

	if (!_loc->allowsMethod(req.method()))
		return (_handler->serveError(HS_METHOD_NOT_ALLOWED, _loc, res));

	if (req.method() == "GET")
		return (handleGet(req, res));
	else if (req.method() == "POST")
		return (handlePost(req, res));
	else if (req.method() == "DELETE")
		return (handleDelete(req, res));
	else
		return (_handler->serveError(HS_NOT_IMPLEMENTED, _loc, res));

}

void MethodHandler::handleGet(Http::Request & req, Http::Response & res)
{
	(void)req;
	StaticHandler * static_handler = new StaticHandler(_handler);
	if (!static_handler->handle(_path, &res))
	{
		delete static_handler;
		return (_handler->serveError(_loc, res));
	}
	if (static_handler->fd() == IN_MEMORY_FD)
	{
		delete static_handler;
		return (_handler->setStatus(HS_OK));
	}
	return (EventLoop::instance().addHandler(static_handler, POLLIN));
}

void MethodHandler::handlePost(Http::Request & req, Http::Response & res)
{

	const std::string & ct = req.header("Content-Type");
	if (ct.find("application/x-www-form-urlencoded") != std::string::npos
		|| ct.find("multipart/form-data") != std::string::npos)
	{
		const std::vector<Http::RequestBody> & body_fields = req.bodyFields();
		for (size_t i = 0; i < body_fields.size(); ++i)
		{
			if (!body_fields[i].filename.empty())
			{

				continue ; // TODO: handle file upload
			}	
			res.appendBody(
				"<p>" + body_fields[i].field + ": " + body_fields[i].value + "</p>"
			);
		}
	}
	else
	{
		return (_handler->serveError(HS_UNSUPPORTED_MEDIA_TYPE, _loc, res));
	}

}

void MethodHandler::handleDelete(Http::Request & req, Http::Response & res)
{

	(void)req;
	if (remove(_path.c_str()) != 0)
		return (_handler->serveError(HS_FORBIDDEN, _loc, res));
	res.setStatus(200);
	res.setReason("OK");
	res.appendBody("<html><body><h1>File deleted successfully</h1></body></html>");
	return (_handler->setStatus(HS_OK));

}
