#include "webserv.hpp"

using namespace Handler;

UploadHandler::UploadHandler(RequestHandler * handler, const std::string & path, const LocationConfig * loc)
	: EventHandler(-1) , _handler(handler), _path(path), _loc(loc), _file(NULL) {}

UploadHandler::~UploadHandler() {}

bool UploadHandler::upload(const std::string &file, const std::string &data, Http::Response &res)
{

	LOG("Uploading file: " + file);
	const std::string store = _loc->upload_store;
	if (store.empty() || access(store.c_str(), W_OK) != 0)
	{
		_handler->setStatus(HS_FORBIDDEN);
		return (false);
	}
	
	if (file.empty() || file.find("/") != std::string::npos || file.find("\\") != std::string::npos)
	{
		_handler->setStatus(HS_UPROCESSABLE_ENTITY);
		return (false);
	}

	std::string file_path = UriHandler::buildPath(store, _loc);
	if (file_path[file_path.size() - 1] != '/')
		file_path += "/";
	file_path += file;

	_file = FileFactory::create(file_path);
	if (!_file)
	{
		_handler->setStatus(HS_INTERNAL_SERVER_ERROR);
		return (false);
	}

	_file_buf.append(data);
	_file_buf.hasWritten(data.size());
	res.appendBody("Upload: " + file + " successful");
	return (true);

}


void UploadHandler::handle(short e)
{

	LOG("UploadHandler::handle: event " + String::str(e) + " on fd " + String::str(fd()));
	if (e & (POLLERR | POLLHUP | POLLNVAL))
	{
		EventLoop::instance().delHandler(this);
		ERR("UploadHandler::handle: poll error");
    return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
	}

	if (e & POLLOUT)
	{

		try
		{
			ssize_t n = _file->write(_file_buf.readPtr(), 4096);
			if (n <= 0)
			{
				EventLoop::instance().delHandler(this);
				ERR("UploadHandler::handle: write error");
				return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
			}
			_file_buf.hasRead(n);
			if (_file_buf.readable() == 0)
			{
				LOG(_file->getPath() + " uploaded successfully");
				EventLoop::instance().delHandler(this);
				_handler->setStatus(HS_OK);
			}
		}
		catch(const std::exception& e)
		{
			ERR("UploadHandler::handle: " + std::string(e.what()));
			EventLoop::instance().delHandler(this);
			return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
		}
	}

}
