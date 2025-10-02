#include "webserv.hpp"

using namespace Handler;

RequestHandler::RequestHandler(Net::Client * client)
	: _client(client), _state(HS_PROGRESS)
{
}

RequestHandler::~RequestHandler()
{
}

HandlerState	RequestHandler::state() const { return (_state); }
void	RequestHandler::setState(HandlerState state) { _state = state; }

void	RequestHandler::handle(Http::Request & req, Http::Response & res)
{

	const std::string & method = req.method();
	const std::string & uri = req.uri();

	mergeHeaders(req, res);
	const LocationConfig * loc = findLocation(uri);
	if (!loc)
	{
		//TODO custom error page
		res.setStatusCode(404);
		res.setReason("Not Found");
		res.appendBody("404 Not Found");
		res.setHeader("Content-Length", String::str(res.body().length()));
		setState(HS_DONE);
		return ;
	}
	LOG("Matched location: " + loc->path + " for uri: " + uri);
	if (!loc->allowsMethod(method))
	{
		//TODO custom error page
		res.setStatusCode(405);
		res.setReason("Method Not Allowed");
		res.appendBody("405 Method Not Allowed");
		res.setHeader("Content-Length", String::str(res.body().length()));
		setState(HS_DONE);
		return ;
	}
	if (loc->redirect.first != 0)
	{
		res.setStatusCode(loc->redirect.first);
		res.setReason("Redirect");
		res.setHeader("Location", loc->redirect.second);
		setState(HS_DONE);
		return ;
	}
	setState(HS_DONE); //TODO actual handling
	res.setStatusCode(200);
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
	res.setHeader("Server", "webserv/0.1");
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
	_state = HS_PROGRESS;
}