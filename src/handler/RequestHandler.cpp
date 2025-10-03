#include "webserv.hpp"

using namespace Handler;

RequestHandler::RequestHandler(Net::Client * client)
	: _client(client), _state(HS_PROGRESS)
{
}

RequestHandler::~RequestHandler()
{
}

State	RequestHandler::state() const { return (_state); }
void	RequestHandler::setState(State state) { _state = state; }

void	RequestHandler::handle(Http::Request & req, Http::Response & res)
{

	ErrorHandler error_handler(this);
	const std::string & method = req.method();
	const std::string & uri = req.uri();

	mergeHeaders(req, res);
	const LocationConfig * loc = findLocation(uri);

	if (!loc->allowsMethod(method))
	{
		setState(HS_FORBIDDEN);
		return (error_handler.handle(loc, res));
	}

	if (loc->redirect.first != 0)
	{
		res.setStatus(loc->redirect.first);
		res.setReason("Redirect");
		res.setHeader("Location", loc->redirect.second);
		setState(HS_OK);
		return ;
	}

	UriHandler uri_handler(uri, loc, this);
	std::string path = uri_handler.buildPath();
	if (state() == HS_NOT_FOUND || state() == HS_FORBIDDEN)
		return (error_handler.handle(loc, res));

	setState(HS_OK); //TODO actual handling
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

void	RequestHandler::reset() { _state = HS_PROGRESS; }

Net::Client *	RequestHandler::client() const { return (_client); }