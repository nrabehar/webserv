#include "webserv.hpp"

using namespace Handler;

RequestHandler::RequestHandler(Net::Client * client)
	: _client(client), _status(HS_PROGRESS), _error_handler(this),
		_req(NULL), _res(NULL)
{
}

RequestHandler::~RequestHandler()
{

	if (!Signal::terminate)
	{
		for (size_t i = 0; i < _process.size(); ++i)
			EventLoop::instance().delHandler(_process[i]);
	}
	_process.clear();

}

Status	RequestHandler::status() const { return (_status); }
void	RequestHandler::addProcess(IEventHandler * h, short e)
{
	_process.push_back(h);
	EventLoop::instance().addHandler(h, e);
}
void	RequestHandler::delProcess(IEventHandler * h)
{

	for (size_t i = 0; i < _process.size(); ++i)
	{
		if (_process[i] == h)
		{
			_process.erase(_process.begin() + i);
			break ;
		}
	}
	EventLoop::instance().delHandler(h);
}

void	RequestHandler::setStatus(Status state)
{

	_status = state;
	if (state == HS_OK && _process.size())
		_status = HS_PROGRESS;
	else if (isError())
	{
		for (size_t i = 0; i < _process.size(); ++i)
			EventLoop::instance().delHandler(_process[i]);
		_process.clear();
		if (!_req || !_res)
			return ;
		const LocationConfig * loc = findLocation(_req->uri());
		serveError(loc, *_res);
	}

}


bool	RequestHandler::isError() const
{

	int s = (int)_status;
	return (s >= 400 && s < 600);

}

void	RequestHandler::handle(Http::Request & req, Http::Response & res)
{

	_req = &req; _res = &res;
	const std::string & method = req.method();
	const std::string & uri = req.uri();

	mergeHeaders(req, res);
	const LocationConfig * loc = findLocation(uri);

	if (isError())
		return (serveError(loc, res));

	if (!loc->allowsMethod(method))
	{
		setStatus(HS_METHOD_NOT_ALLOWED);
		return (_error_handler.handle(loc, res));
	}
	if (loc->redirect.first != 0)
	  return (redirect(loc->redirect, res));

	UriHandler uri_handler(uri, loc, this);
	std::string path = uri_handler.buildPath();
	if (isError())
		return (_error_handler.handle(loc, res));
	MethodHandler method_handler(this, path, loc);
	method_handler.handle(req, res);
}

bool	RequestHandler::isCgiRequest(Http::Request & req)
{

	const LocationConfig * loc = findLocation(req.uri());

	UriHandler	uri_handler(req.uri(), loc, this);
	if (isError())
		return (false);
	return (status() == HS_CGI);

}

const LocationConfig *	RequestHandler::findLocation(const std::string & uri) const
{

	Net::Server * server = _client->getServer();
	const ServerConfig & conf = server->getConfig();
	const std::vector<LocationConfig> & loc = conf.location;

	const LocationConfig * best_match = NULL;
	size_t best_len = 0;
	std::vector<LocationConfig>::const_iterator it;
	for (it = loc.begin(); it != loc.end(); ++it)
	{
		const LocationConfig & l = *it;
		size_t len = l.path.length();
		if (len > best_len && uri.compare(0, len, l.path) == 0)
		{
			best_match = &l;
			best_len = len;
		}
	}

	return (best_match);

}

void	RequestHandler::mergeHeaders(Http::Request & req, Http::Response & res)
{

	res.setVersion(req.version());
	// res.setHeader("Date", String::httpDate(time(NULL)));
	res.setHeader("Server", "webserv");
	if (req.header().find("Connection") != req.header().end())
		res.setHeader("Connection", req.header("Connection"));
	else if (res.version() == "HTTP/1.1")
		res.setHeader("Connection", "keep-alive");
	else
		res.setHeader("Connection", "close");
	res.setHeader("Content-Type", "text/html");

}

void	RequestHandler::reset()
{
	_status = HS_PROGRESS;
	_req = NULL;
	_res = NULL;
}

Net::Client *	RequestHandler::client() const { return (_client); }

void	RequestHandler::serveError(Status status, const LocationConfig * loc, Http::Response & res)
{
	_status = status;
	serveError(loc, res);
}

void	RequestHandler::serveError(const LocationConfig * loc, Http::Response & res)
{
	_error_handler.handle(loc, res);
}

void RequestHandler::serveDirectory(const std::string & path, const std::string & uri, Http::Response & res)
{

	if (access(path.c_str(), R_OK) != 0)
	  return (serveError(HS_FORBIDDEN, NULL, res));

	std::string clean_uri = uri;
	if (uri.find("?") != std::string::npos)
		clean_uri = clean_uri.substr(0, uri.find("?"));

	std::string body = UI::getDirListing(path, clean_uri);
	res.setStatus(HS_OK);
	res.setReason("OK");
	res.setHeader("Content-Type", "text/html");
	res.setHeader("Content-Length", String::str(body.length()));
	res.appendBody(body);
	setStatus(HS_OK);
}

void RequestHandler::redirect(const std::pair<int, std::string> & redirect, Http::Response & res)
{

	res.setStatus(redirect.first);
	res.setReason("Redirect");
	res.setHeader("Location", redirect.second);
	setStatus(HS_OK);

}
