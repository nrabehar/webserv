#include "webserv.hpp"

using namespace Handler;

MethodHandler::MethodHandler(RequestHandler * handler, const std::string & path, const LocationConfig * loc)
	: _handler(handler), _path(path), _loc(loc) {}

MethodHandler::~MethodHandler() {}

void MethodHandler::handle(Http::Request & req, Http::Response & res)
{

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
	const LocationConfig * loc = _handler->findLocation(req.uri());
	UriHandler uri_h(req.uri(), loc, _handler);
	_path = uri_h.buildPath();
	if (_handler->isError())
		return (_handler->serveError(loc, res));
	if (_handler->status() == HS_FOLDER_LISTING)
		return (_handler->serveDirectory(_path, req.uri(), res));
	res.setHeader("Content-Type", Mime::getType(_path));
	std::ifstream file(_path.c_str(), std::ios::in | std::ios::binary);
	if (!file.good())
		return (_handler->serveError(HS_INTERNAL_SERVER_ERROR, _loc, res));
	std::list<std::string> contents;
	char buffer[4096];
	while (file.read(buffer, sizeof(buffer)))
		contents.push_back(std::string(buffer, sizeof(buffer)));
	contents.push_back(std::string(buffer, file.gcount()));
	file.close();
	size_t total_size = 0;
	for (std::list<std::string>::iterator it = contents.begin(); it != contents.end(); ++it)
		total_size += it->size();
	res.setHeader("Content-Length", String::str(total_size));
	std::string headers = res.str();
	total_size += headers.size() + 1;
	try
	{
		size_t offset = 0;
		char * buf = new char[total_size];
		std::memset(buf, 0, total_size);
		std::memmove(buf, headers.c_str(), headers.size());
		offset += headers.size();
		for (std::list<std::string>::iterator it = contents.begin(); it != contents.end(); ++it)
		{
			std::memmove(buf + offset, it->c_str(), it->size());
			offset += it->size();
		}
		_handler->client()->setOut(buf, total_size);
	}
	catch(const std::exception& e)
	{
		(void)e;
		ERR("Error while preparing file content: " + std::string(e.what()));
		return (_handler->serveError(HS_INTERNAL_SERVER_ERROR, _loc, res));
	}
	
}

void MethodHandler::handlePost(Http::Request & req, Http::Response & res)
{

	const std::string & ct = req.header("Content-Type");
	if (ct.find("application/x-www-form-urlencoded") != std::string::npos
		|| ct.find("multipart/form-data") != std::string::npos)
	{
		std::vector<std::pair<std::string, std::string> > files;
		std::vector<std::pair<std::string, std::string> > fields;
		const std::vector<Http::RequestBody> & body_fields = req.bodyFields();
		for (size_t i = 0; i < body_fields.size(); ++i)
		{
			if (!body_fields[i].filename.empty())
			{
				if (_loc->upload_store.empty())
				{
					ERR("Upload store not configured for location");
					return (_handler->setStatus(HS_FORBIDDEN));
				}
				std::string filename = _loc->upload_store + Time::timeToStr(Time::now(), "%Y%m%d%H%M%S-");
				filename += body_fields[i].filename;
				if (std::rename(body_fields[i].value.c_str(), filename.c_str()) < 0)
				{
					std::remove(body_fields[i].value.c_str());
					ERR("Could not move uploaded file to destination: " + std::string(strerror(errno)));
					return (_handler->setStatus(HS_INTERNAL_SERVER_ERROR));
				}
				files.push_back(std::make_pair(body_fields[i].field, filename));
				res.setStatus(201);
			}
			else
				fields.push_back(std::make_pair(body_fields[i].field, body_fields[i].value));
		}
		std::string body;
		createJsonResponse(files, fields, body);
		res.setHeader("Content-Length", String::str(body.size()));
		res.setHeader("Content-Type", "application/json");
		std::string headers = res.str();
		size_t total_size = headers.size() + 1 + body.size();
		char * buf = new char[total_size];
		std::memset(buf, 0, total_size);
		size_t offset = 0;
		std::memmove(buf, headers.c_str(), headers.size());
		offset += headers.size();
		std::memmove(buf + offset, body.c_str(), body.size());
		_handler->client()->setOut(buf, total_size);
		_handler->setStatus(HS_OK);
	}
	else
	{
		res.setHeader("Content-type", req.header("Content-Type"));
		res.setHeader("Content-Length", String::str(req.body().size()));
		size_t total_size = res.str().size() + 1 + req.body().size();
		char * buf = new char[total_size];
		std::memset(buf, 0, total_size);
		size_t offset = 0;
		std::string headers = res.str();
		std::memmove(buf, headers.c_str(), headers.size());
		offset += headers.size();
		std::memmove(buf + offset, req.body().c_str(), req.body().size());
		_handler->client()->setOut(buf, total_size);
		_handler->setStatus(HS_OK);
	}

}

void MethodHandler::handleDelete(Http::Request & req, Http::Response & res)
{

	const LocationConfig * loc = _handler->findLocation(req.uri());
	UriHandler uri_h(req.uri(), loc, _handler);
	_path = uri_h.buildPath();
	if (_handler->isError())
		return (_handler->serveError(loc, res));
	if (remove(_path.c_str()) != 0)
		return (_handler->serveError(HS_INTERNAL_SERVER_ERROR, _loc, res));
	res.setStatus(200);
	res.setHeader("Content-Type", "application/json");
	std::string body = "{\"message\": \"File deleted successfully\"}\n";
	res.setHeader("Content-Length", String::str(body.size()));
	std::string headers = res.str();
	size_t total_size = headers.size() + 1 + body.size();
	char * buf = new char[total_size];
	std::memset(buf, 0, total_size);
	size_t offset = 0;
	std::memmove(buf, headers.c_str(), headers.size());
	offset += headers.size();
	std::memmove(buf + offset, body.c_str(), body.size());
	_handler->client()->setOut(buf, total_size);
	return (_handler->setStatus(HS_OK));

}

void MethodHandler::createJsonResponse(const std::vector<std::pair<std::string, std::string> > & files,
	const std::vector<std::pair<std::string, std::string> > & fields, std::string & res)
{
	res.append("{\n");

	res.append("  \"files\": [\n");
	for (size_t i = 0; i < files.size(); ++i)
	{
		res.append("    {\n");
		res.append("      \"field\": \"" + files[i].first + "\",\n");
		res.append("      \"filename\": \"" + files[i].second + "\"\n");
		if (i + 1 < files.size())
			res.append("    },\n");
		else
			res.append("    }\n");
	}
	res.append("  ],\n");

	res.append("  \"fields\": [\n");
	for (size_t i = 0; i < fields.size(); ++i)
	{
		res.append("    {\n");
		res.append("      \"field\": \"" + fields[i].first + "\",\n");
		res.append("      \"value\": \"" + fields[i].second + "\"\n");
		if (i + 1 < fields.size())
			res.append("    },\n");
		else
			res.append("    }\n");
	}
	res.append("  ]\n}\n");
}
