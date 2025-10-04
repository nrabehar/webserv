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
	{
		StaticHandler * static_handler = new StaticHandler(_handler);
		if (!static_handler->handle(_path, &res))
		{
			ERR("failed to serve static file: " + _path);
			delete static_handler;
			return (_handler->serveError(HS_NOT_FOUND, _loc, res));
		}
		LOG("serving static file: " + _path);
		if (static_handler->fd() == -1)
		{
			delete static_handler;
			return (_handler->setStatus(HS_OK));
		}
		return (EventLoop::instance().addHandler(static_handler, POLLIN));
	}
	// else if (req.method() == "POST")
	// {
	// 	// Handle POST method (not implemented in this snippet)
	// 	return (_handler->setStatus(HS_NOT_IMPLEMENTED));
	// }
	else if (req.method() == "DELETE")
	{
		if (remove(_path.c_str()) != 0)
			return (_handler->serveError(HS_FORBIDDEN, _loc, res));
		res.setStatus(200);
		res.setReason("OK");
		res.appendBody("<html><body><h1>File deleted successfully</h1></body></html>");
		return (_handler->setStatus(HS_OK));
	}
	else
		return (_handler->serveError(HS_NOT_IMPLEMENTED, _loc, res));

}
