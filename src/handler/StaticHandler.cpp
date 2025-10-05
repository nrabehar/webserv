#include "webserv.hpp"

using namespace Handler;

StaticHandler::StaticHandler(RequestHandler * handler)
	: EventHandler(-1), _path(""), _offset(0), _file(NULL),
		_handler(handler), _response(NULL), _loc(NULL) {}

StaticHandler::~StaticHandler()
{
	if (_file)
		delete _file;
}

void	StaticHandler::handle(short e)
{

	LOG("StaticHandler::handle: event " + String::str(e) + " on fd " + String::str(fd()));
	if (e & (POLLERR | POLLHUP | POLLNVAL))
	{
		EventLoop::instance().delHandler(this);
		ERR("StaticHandler::handle: poll error");
    return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
	}

	if (e & POLLIN)
	{
		try
		{
			ssize_t n = _file->read();
			if (n <= 0 && !_file->isComplete())
			{
				EventLoop::instance().delHandler(this);
				ERR("StaticHandler::handle: read error or connection closed");
			  return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
			}
			_response->appendBody(_file->getData().substr(_offset, n));
			_offset += n;
			LOG("StaticHandler::handle: read " + String::str(n) + " bytes " + _file->getPath());
			if (_file->isComplete())
			{
				LOG("StaticHandler::handle: file read complete");
				_response->setHeader("Content-Length", String::str(_response->body().length()));
				EventLoop::instance().delHandler(this);
				return (_handler->setStatus(HS_OK));
			}
		}
		catch (std::exception & e)
		{
			ERR("StaticHandler::handle: " + std::string(e.what()));
			EventLoop::instance().delHandler(this);
			return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
		}
	}

}

bool StaticHandler::handle(const std::string & path, Http::Response * res)
{

	_path = path;
	_response = res;
	_offset = 0;

	if (_file)
	{
		delete _file;
		_file = NULL;
	}

	_file = FileFactory::create(_path);
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


