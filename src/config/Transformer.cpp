#include "webserv.hpp"

Config::Transformer::Transformer() {}
Config::Transformer::~Transformer() {}

std::vector<Config::Server>	Config::Transformer::transform(Node<Token> * root)
{

	std::vector<Server>	servers;

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
			Server server;
			parseServer(node, server);
			servers.push_back(server);
		}

		else
		{
	
			std::vector<Config::Server> nested = transform(node);
			servers.insert(servers.end(), nested.begin(), nested.end());

		}

	}

	return (servers);

}

void Config::Transformer::parseServer(Node<Token> * svr_node, Server & svr)
{

	const std::vector<Node<Token>* > & child = svr_node->getChild();

	for (size_t i = 0; i < child.size(); ++i)
	{

		Node<Token> * node = child[i];
		if (node->getName() == "listen")
		{

			Server::Listen listen;
			parseListen(node, listen);
			svr.listen.push_back(listen);

		}
		else if (node->getName() == "location")
		{

			Server::Location location;
			parseLocation(node, location);
			svr.location.push_back(location);

		}
		else if (node->getName() == "keepalive_timeout")
		{
			svr.keepalive_timeout = static_cast<size_t>(std::atoi(node->getData()[0].value.c_str()));		
		}

	}

}

void Config::Transformer::parseListen(Node<Token> * listen_node, Server::Listen & listen)
{

	const std::vector<Token> & arg = listen_node->getData();
	if (arg.size() >= 1)
		listen.port = arg[0].value;
	if (arg.size() >= 2)
		listen.host = arg[1].value;

	// ! @todo make full parsing here or do it on validator

}

void Config::Transformer::parseLocation(Node<Token> * loc_node, Server::Location & loc)
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

	}
	
}
