#ifndef TRANSFORMER_HPP
#define TRANSFORMER_HPP

#include "../webserv.hpp"

class Config::Transformer
{

	public:

		Transformer();
		~Transformer();

		static std::vector<ServerConfig> transform(Node<Token>* root);

	private:

		Transformer(const Transformer &);
		Transformer & operator=(const Transformer &);

		static void parseServer(Node<Token>* svr_node, ServerConfig& svr);
		static void parseListen(Node<Token>* listen_node, ServerConfig::Listen& listen);
		static void parseLocation(Node<Token>* loc_node, LocationConfig& loc);

};

#endif // TRANSFORMER_HPP
