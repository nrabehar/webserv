#ifndef TRANSFORMER_HPP
#define TRANSFORMER_HPP

#include "../webserv.hpp"

class Config::Transformer
{

	public:

		Transformer();
		~Transformer();

		static std::vector<Config::Server> transform(Node<Token>* root);

	private:

		Transformer(const Transformer &);
		Transformer & operator=(const Transformer &);

		static void parseServer(Node<Token>* svr_node, Server& svr);
		static void parseListen(Node<Token>* listen_node, Server::Listen& listen);
		static void parseLocation(Node<Token>* loc_node, Server::Location& loc);

};

#endif // TRANSFORMER_HPP
