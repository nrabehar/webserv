#include "webserv.hpp"

using namespace Handler;

UploadHandler::UploadHandler(RequestHandler * handler, const std::string & path, const LocationConfig * loc)
	: EventHandler(-1) , _handler(handler), _path(path), _loc(loc), _file(NULL) {}

UploadHandler::~UploadHandler()
{

	if (_file)
		delete _file;

}

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
		_handler->setStatus(HS_CONFLICT);
		return (false);
	}

	std::string file_path = store;
	if (file_path[file_path.size() - 1] != '/')
		file_path += "/";
	file_path += file;

	_file = FileFactory::create(file_path, O_CREAT | O_WRONLY | O_TRUNC);
	if (!_file)
	{
		ERR("Failed to create file: " + file_path);
		_handler->setStatus(HS_INTERNAL_SERVER_ERROR);
		return (false);
	}

	_file_buf.append(data);
	res.appendBody("Upload: " + file + " successful");
	_fd = _file->fd();
	return (true);

}


void UploadHandler::handle(short e)
{

	LOG("UploadHandler::handle: event " + String::str(e) + " on fd " + String::str(fd()));

	if (e & POLLOUT)
	{

		try
		{
			ssize_t n = _file->write(_file_buf.readPtr(), _file_buf.readable());
			if (n <= 0)
			{
				ERR("UploadHandler::handle: write error");
				return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
			}
			_file_buf.hasRead(n);
			if (_file_buf.readable() == 0)
			{
				LOG(_file->getPath() + " uploaded successfully");
				_handler->delProcess(this);
				_handler->setStatus(HS_OK);
			}
		}
		catch(const std::exception& e)
		{
			ERR("UploadHandler::handle: " + std::string(e.what()));
			return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
		}
	}
	if (e & (POLLERR | POLLHUP | POLLNVAL))
	{
		ERR("UploadHandler::handle: poll error");
    return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
	}

}
