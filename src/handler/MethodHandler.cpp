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
	// Handle POST method (not implemented in this snippet)
	return (_handler->setStatus(HS_NOT_IMPLEMENTED));
}

void MethodHandler::handleDelete(Http::Request & req, Http::Response & res)
{
	if (remove(_path.c_str()) != 0)
		return (_handler->serveError(HS_FORBIDDEN, _loc, res));
	res.setStatus(200);
	res.setReason("OK");
	res.appendBody("<html><body><h1>File deleted successfully</h1></body></html>");
	return (_handler->setStatus(HS_OK));
}
