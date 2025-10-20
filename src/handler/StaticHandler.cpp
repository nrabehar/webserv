#include "webserv.hpp"
#include "core/memory/memory.hpp"

using namespace Handler;

StaticHandler::StaticHandler(RequestHandler * handler)
  : _fd(-1), _path(""), _offset(0), _file(NULL),
    _handler(handler), _response(NULL), _loc(NULL) {}

StaticHandler::~StaticHandler()
{
  if (_file)
    ft::free(_file);
}

int StaticHandler::fd() const
{
  return (_fd);
}

void  StaticHandler::handle(short e)
{

  if (e & (POLLERR | POLLHUP | POLLNVAL))
    return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));

  if (e & POLLIN)
  {
    try
    {
      ssize_t n = _file->read();
      if (n <= 0 && !_file->isComplete())
      {
        _handler->delProcess(this);
        return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
      }

      _response->appendBody(_file->getData().substr(_offset, n));
      _offset += n;
      if (_file->isComplete())
      {
        _response->setHeader("Content-Length", String::str(_response->body().length()));
        _handler->delProcess(this);
        return (_handler->setStatus(HS_OK));
      }
    }
    catch (std::exception & e)
    {
      _handler->delProcess(this);
      return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
    }
  }

}

void  StaticHandler::onTimeout()
{

  if (_handler->status() == HS_REQUEST_TIMEOUT)
  {
    if (_file)
      ft::free(_file);
    _file = NULL;
    _handler->delProcess(this);
    return;
  }

}

bool StaticHandler::handle(const std::string & path, Http::Response * res)
{

  _path = path;
  _response = res;
  _offset = 0;

  if (_file)
  {
    ft::free(_file);
    _file = NULL;
  }
  _file = FileFactory::create(_path, O_RDONLY);
  if (!_file)
  {
    _handler->setStatus(HS_INTERNAL_SERVER_ERROR);
    return (false);
  }
  _fd = _file->fd();
  if (_fd == IN_MEMORY_FD)
  {
    _response->appendBody(_file->getData());
    _response->setHeader("Content-Length", String::str(_response->body().length()));
  }
  _response->setHeader("Content-Type", Mime::getType(_path));

  return (true);

}
