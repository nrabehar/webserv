#include "webserv.hpp"

using namespace Handler;

RequestHandler::RequestHandler(Net::Client * client, Http::Request * req, Http::Response * res)
  : _client(client), _status(HS_WAITING), _error_handler(this),
    _cgi_handler(), _req(req), _res(res)
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

Status  RequestHandler::status() const { return (_status); }

void  RequestHandler::notifyTimeout()
{

  for (size_t i = 0; i < _process.size(); ++i)
    _process[i]->onTimeout();

}

void  RequestHandler::addProcess(IEventHandler * h, short e)
{
  _process.push_back(h);
  EventLoop::instance().addHandler(h, e);
}

void  RequestHandler::delProcess(IEventHandler * h)
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

void  RequestHandler::setStatus(Status state)
{

  _status = state;
  if (state == HS_OK && _process.size())
    _status = HS_PROGRESS;

}

bool  RequestHandler::isError() const
{

  int s = (int)_status;
  return (s >= 400 && s < 600);

}

void  RequestHandler::handle(Http::Request & req, Http::Response & res)
{

  _req = &req; _res = &res;
  const std::string & uri = req.uri();

  mergeHeaders(req, res);
  std::cout << __FILE__ << ": " << __LINE__ << std::endl;
  const LocationConfig * loc = findLocation(uri);

  if (isError())
    return (serveError(loc, res));

  if (loc->redirect.first != 0)
    return (redirect(loc->redirect, res));

  if (isCgiRequest(req))
  {
    if (isError())
      return (serveError(loc, res));
    return ;
  }

  UriHandler uri_handler(uri, loc, this);
  std::string path = uri_handler.buildPath();
  MethodHandler method_handler(this, path, loc);
  method_handler.handle(req, res);
}

bool  RequestHandler::isCgiRequest(Http::Request & req)
{

  std::cout << __FILE__ << ": " << __LINE__ << std::endl;
  const LocationConfig * loc = findLocation(req.uri());

  Status st = status();
  UriHandler  uri_handler(req.uri(), loc, this);
  uri_handler.buildPath();
  if (status() == HS_CGI)
    return (true);
  setStatus(st);
  return (false);

}

bool  RequestHandler::initCgiHandler(Http::Request & req, Http::Response & res)
{

  std::cout << __FILE__ << ": " << __LINE__ << std::endl;
  const LocationConfig * loc = findLocation(req.uri());
  if (!loc)
  {
    setStatus(HS_INTERNAL_SERVER_ERROR);
    return (false);
  }

  UriHandler  uri_handler(req.uri(), loc, this);
  std::string path = uri_handler.buildPath();

  std::string ext;
  size_t dot = path.rfind('.');
  ext = path.substr(dot);

  std::string bin;
  for (size_t i = 0; i < loc->cgi.size(); ++i)
  {
    if (loc->cgi[i].first == ext)
    {
      bin = loc->cgi[i].second;
      break ;
    }
  }

  if (bin.empty() || path.empty()
    || access(path.c_str(), F_OK) != 0
    || access(bin.c_str(), F_OK) != 0
    || access(bin.c_str(), X_OK) != 0)
  {
    setStatus(HS_INTERNAL_SERVER_ERROR);
    return (false);
  }

  size_t  timeout = _client->getServer()->getConfig().gateway_timeout;
  _cgi_handler = CgiHandler(this, &req, &res, timeout);
  _cgi_handler.launch(bin, path);
  if (isError())
  {
    _cgi_handler = CgiHandler();
    return (false);
  }
  return (true);

}

CgiHandler * RequestHandler::cgiHandler() { return (&_cgi_handler); }

const LocationConfig *  RequestHandler::findLocation(const std::string & uri) const
{

  std::string c_uri = uri;
  if (c_uri.find('?') != std::string::npos)
    c_uri = c_uri.substr(0, c_uri.find('?'));
  Net::Server * server = _client->getServer();
  const ServerConfig conf = server->getConfig();
  const std::vector<LocationConfig> loc = conf.location;
  const LocationConfig * best_match = NULL;
  size_t best_len = 0;
  std::vector<LocationConfig>::const_iterator it;
  for (it = loc.begin(); it != loc.end(); ++it)
  {
    const LocationConfig * l = &(*it);
    if (!l)
    {
      break ;
    }
    size_t len = l->path.length();
    if (c_uri == l->path)
    {
      return (l);
    }
    if (len > best_len && c_uri.compare(0, len, l->path) == 0)
    {
      best_match = l;
      best_len = len;
    }
  }
  return (best_match);
}

void  RequestHandler::mergeHeaders(Http::Request & req, Http::Response & res)
{

  res.setVersion(req.version());
  res.setHeader("Date", Time::timeToHttpStr());
  res.setHeader("Server", "webserv");
  if (req.header().find("Connection") != req.header().end())
    res.setHeader("Connection", req.header("Connection"));
  else if (res.version() == "HTTP/1.1")
    res.setHeader("Connection", "keep-alive");
  else
    res.setHeader("Connection", "close");
  res.setHeader("Content-Type", "text/html");

}

void  RequestHandler::reset()
{
  _req->cleanup();
  _status = HS_WAITING;
}

Net::Client * RequestHandler::client() const { return (_client); }
Http::Response *  RequestHandler::response() { return (_res); }

void  RequestHandler::serveError(Status status, const LocationConfig * loc, Http::Response & res)
{
  _status = status;
  serveError(loc, res);
}

void  RequestHandler::serveError(const LocationConfig * loc, Http::Response & res)
{

  if (!isError() || !loc)
    return ;
  int st = static_cast<int>(_status);
  if (st == 400 || st == 408 || st >= 500)
    res.setHeader("Connection", "close");
  res.setStatus(st);
  _req->cleanup();
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
  std::string headers = res.str();
  size_t total_size = headers.size() + 1 + body.size();
  char * buf = new char[total_size];
  std::memset(buf, 0, total_size);
  size_t offset = 0;
  std::memmove(buf, headers.c_str(), headers.size());
  offset += headers.size();
  std::memmove(buf + offset, body.c_str(), body.size());
  _client->setOut(buf, total_size);
  setStatus(HS_OK);
}

void RequestHandler::redirect(const std::pair<int, std::string> & redirect, Http::Response & res)
{

  res.setStatus(redirect.first);
  res.setReason("Redirect");
  res.setHeader("Location", redirect.second);
  setStatus(HS_OK);

}
