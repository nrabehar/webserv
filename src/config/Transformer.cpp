#include "webserv.hpp"

Config::Transformer::Transformer() {}
Config::Transformer::~Transformer() {}

std::vector<ServerConfig>	Config::Transformer::transform(Node<Token> * root)
{

	std::vector<ServerConfig>	servers;

	if (!root)
	{
		WRN("No defined root node");
		return (servers);
	}

	const std::vector<Node<Token>* > & child = root->getChild();

	for (size_t i = 0; i < child.size() ; ++i)
	{

		Node<Token> * node = child[i];
		if (node->getName() == "server")
		{
			ServerConfig server;
			parseServer(node, server);
			servers.push_back(server);
		}

		else
		{

			std::vector<ServerConfig> nested = transform(node);
			servers.insert(servers.end(), nested.begin(), nested.end());

		}

	}

	return (servers);

}

void Config::Transformer::parseServer(Node<Token> * svr_node, ServerConfig & svr)
{

	const std::vector<Node<Token>* > & child = svr_node->getChild();

	for (size_t i = 0; i < child.size(); ++i)
	{

		Node<Token> * node = child[i];
		if (node->getName() == "listen")
		{

			ServerConfig::Listen listen;
			parseListen(node, listen);
			svr.listen.push_back(listen);

		}
		else if (node->getName() == "location")
		{

			LocationConfig location;
			parseLocation(node, location);
			svr.location.push_back(location);

		}
		else if (node->getName() == "keepalive_timeout")
		{
			std::string val = node->getData()[0].value;
			svr.keepalive_timeout = static_cast<size_t>(std::atoi(val.c_str()));
		}
		else if (node->getName() == "gateway_timeout")
		{
			std::string val = node->getData()[0].value;
			svr.gateway_timeout = static_cast<size_t>(std::atoi(val.c_str()));
		}

	}

}

void Config::Transformer::parseListen(Node<Token> * listen_node, ServerConfig::Listen & listen)
{

	const std::vector<Token> & arg = listen_node->getData();
	if (arg.size() == 2)
	{
		if (String::isNumeric(arg[0].value))
		{
			listen.port = arg[0].value;
			listen.host = arg[1].value;
			return ;
		}
		listen.port = arg[1].value;
		listen.host = arg[0].value;
		return ;
	}
	size_t	coln_pos = arg[0].value.find_last_of(':');
	if (coln_pos != std::string::npos)
	{
		listen.port = arg[0].value.substr(coln_pos + 1);
		listen.host = arg[0].value.substr(0, coln_pos);
		return ;
	}
	if (String::isNumeric(arg[0].value))
	{
		listen.port = arg[0].value;
		return ;
	}
	listen.host = arg[0].value;

}

void Config::Transformer::parseLocation(Node<Token> * loc_node, LocationConfig & loc)
{

	loc.path = loc_node->getData()[0].value;

	const std::vector<Node<Token>* > child = loc_node->getChild();
	for (size_t i = 0; i < child.size(); ++i)
	{

		Node<Token> * node = child[i];
		const std::vector<Token> & arg = node->getData();
		const std::string & name = node->getName();

		if (name == "root")
			loc.root = arg[0].value;
		else if (name == "autoindex")
			loc.autoindex = (arg[0].value == "on");
		else if (name == "index")
			for (size_t j = 0; j <  arg.size(); ++j)
				loc.index.push_back(arg[j].value);
		else if (name == "error_page" && arg.size() >= 2)
			for (size_t j = 0; j <  arg.size() - 1; ++j)
				loc.err_page[std::atoi(arg[j].value.c_str())] = arg[arg.size() - 1].value;
		else if (name == "cgi" && arg.size() >= 2)
			loc.cgi.push_back(std::make_pair(arg[0].value, arg[1].value));
		else if (name == "methods")
			for (size_t j = 0; j <  arg.size(); ++j)
				loc.methods.push_back(arg[j].value);
		else if (name == "client_max_body_size")
			// loc.client_max_body_size = static_cast<size_t>(std::atoi(arg[0].value.c_str())) * 1024 * 1024;
			loc.client_max_body_size = String::toByte(arg[0].value);
		else if (name == "upload_store")
		{
			loc.upload_store = arg[0].value;
			if (loc.upload_store[loc.upload_store.size() - 1] != '/')
				loc.upload_store += '/';
		}
		else if (name == "return" && arg.size() == 2)
			loc.redirect = std::make_pair(std::atoi(arg[0].value.c_str()), arg[1].value);

	}

}

void Config::Transformer::validate(std::vector<ServerConfig> & servers)
{

	if (servers.empty())
		throw std::runtime_error("No server defined in configuration");
	for (size_t i = 0; i < servers.size(); ++i)
	{

		ServerConfig & svr = servers[i];
		if (svr.listen.empty())
			throw std::runtime_error("Server has no listen directive");
		std::vector<LocationConfig> & locs = svr.location;
		if (locs.empty())
			throw std::runtime_error("Server has no location directive");
		for (size_t j = 0; j < locs.size(); ++j)
		{
			LocationConfig & loc = locs[j];
			if (loc.root.empty())
				throw std::runtime_error("Location " + loc.path + " has no root directive");
		}
	}

}
