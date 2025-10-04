#include "webserv.hpp"

using namespace Handler;

RequestHandler::RequestHandler(Net::Client * client)
	: _client(client), _status(HS_PROGRESS), _error_handler(this)
{
}

RequestHandler::~RequestHandler()
{
}

Status	RequestHandler::status() const { return (_status); }
void	RequestHandler::setStatus(Status state) { _status = state; }
bool	RequestHandler::isError() const
{

	int s = (int)_status;
	return (s >= 400 && s < 600);

}

void	RequestHandler::handle(Http::Request & req, Http::Response & res)
{

	const std::string & method = req.method();
	const std::string & uri = req.uri();

	mergeHeaders(req, res);
	const LocationConfig * loc = findLocation(uri);

	if (!loc->allowsMethod(method))
	{
		setStatus(HS_METHOD_NOT_ALLOWED);
		return (_error_handler.handle(loc, res));
	}

	if (loc->redirect.first != 0)
	{
		res.setStatus(loc->redirect.first);
		res.setReason("Redirect");
		res.setHeader("Location", loc->redirect.second);
		setStatus(HS_OK);
		return ;
	}

	UriHandler uri_handler(uri, loc, this);
	std::string path = uri_handler.buildPath();
	if (isError())
		return (_error_handler.handle(loc, res));

	setStatus(HS_OK); //TODO actual handling
	res.setStatus(200);
	res.setReason("OK");
	res.appendBody("<html><body><h1>It works!</h1></body></html>");
	res.setHeader("Content-Length", String::str(res.body().length()));

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

void	RequestHandler::reset() { _status = HS_PROGRESS; }

Net::Client *	RequestHandler::client() const { return (_client); }

void	RequestHandler::serveError(Status status, const LocationConfig * loc, Http::Response & res)
{
	setStatus(status);
	serveError(loc, res);
}

void	RequestHandler::serveError(const LocationConfig * loc, Http::Response & res)
{
	_error_handler.handle(loc, res);
}
