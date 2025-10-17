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
	DBG("Serving file for GET: " + _path);
	std::ifstream file(_path.c_str(), std::ios::in | std::ios::binary);
	if (!file.good())
		return (_handler->serveError(HS_INTERNAL_SERVER_ERROR, _loc, res));
	DBG("Reading file content for GET");
	std::list<std::string> contents;
	char buffer[4096];
	while (file.read(buffer, sizeof(buffer)))
		contents.push_back(std::string(buffer, sizeof(buffer)));
	contents.push_back(std::string(buffer, file.gcount()));
	file.close();
	DBG("Finished reading file content for GET");
	size_t total_size = 0;
	for (std::list<std::string>::iterator it = contents.begin(); it != contents.end(); ++it)
		total_size += it->size();
	DBG("Total file size: " + String::str(total_size));
	res.setHeader("Content-Length", String::str(total_size));
	std::string headers = res.str();
	total_size += headers.size() + 1;
	try
	{
		size_t offset = 0;
		char * buf = new char[total_size];
		std::memmove(buf, headers.c_str(), headers.size());
		offset += headers.size();
		for (std::list<std::string>::iterator it = contents.begin(); it != contents.end(); ++it)
		{
			std::memmove(buf + offset, it->c_str(), it->size());
			offset += it->size();
		}
		delete [] buf;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (_handler->serveError(HS_INTERNAL_SERVER_ERROR, _loc, res));
	}
	DBG("File content read successfully, preparing response");
	
	// if (ss.fail())
	// 	return (_handler->serveError(HS_INTERNAL_SERVER_ERROR, _loc, res));
	// file.close();
	// DBG("File content read successfully, preparing response");
	// try
	// {
	// 	DBG("Reading file content for GET: " + _path);
	// 	res.appendBody(file_content);
	// 	DBG("File content size: " + String::str(res.body().size()));
	// 	res.setHeader("Content-Length", String::str(res.body().size()));
	// 	res.setHeader("Last-Modified", Time::timeToHttpStr(Time::now()));
	// 	DBG("GET request served successfully");
	// 	_handler->setStatus(HS_OK);
	// }
	// catch(const std::exception& e)
	// {
	// 	ERR("Error while reading file content: " + std::string(e.what()));
	// 	return (_handler->serveError(HS_INTERNAL_SERVER_ERROR, _loc, res));
	// }
	res.appendBody("<html><body><h1>File content serving not implemented yet</h1></body></html>");
	res.setHeader("Content-Length", String::str(res.body().size()));
	_handler->setStatus(HS_OK);
	
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
		createJsonResponse(files, fields, res);
		res.setHeader("Content-Length", String::str(res.body().size()));
		_handler->setStatus(HS_OK);
	}
	else
	{
		res.setHeader("Content-type", req.header("Content-Type"));
		res.appendBody(req.body());
		res.setHeader("Content-Length", String::str(req.body().size()));
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
	res.setReason("OK");
	res.appendBody("<html><body><h1>File deleted successfully</h1></body></html>");
	return (_handler->setStatus(HS_OK));

}

void MethodHandler::createJsonResponse(const std::vector<std::pair<std::string, std::string> > & files,
	const std::vector<std::pair<std::string, std::string> > & fields, Http::Response & res)
{
	res.setHeader("Content-Type", "application/json");
	res.appendBody("{\n");

	res.appendBody("  \"files\": [\n");
	for (size_t i = 0; i < files.size(); ++i)
	{
		res.appendBody("    {\n");
		res.appendBody("      \"field\": \"" + files[i].first + "\",\n");
		res.appendBody("      \"filename\": \"" + files[i].second + "\"\n");
		if (i + 1 < files.size())
			res.appendBody("    },\n");
		else
			res.appendBody("    }\n");
	}
	res.appendBody("  ],\n");

	res.appendBody("  \"fields\": [\n");
	for (size_t i = 0; i < fields.size(); ++i)
	{
		res.appendBody("    {\n");
		res.appendBody("      \"field\": \"" + fields[i].first + "\",\n");
		res.appendBody("      \"value\": \"" + fields[i].second + "\"\n");
		if (i + 1 < fields.size())
			res.appendBody("    },\n");
		else
			res.appendBody("    }\n");
	}
	res.appendBody("  ]\n}\n");
}
