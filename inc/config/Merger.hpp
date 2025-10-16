#ifndef CONFIGMERGER_HPP
#define CONFIGMERGER_HPP

#include "../webserv.hpp"

class Config::Merger
{

	public:
		
		Merger();
		~Merger();

		static void	merge(Node<Token> *);
		static void	defLocation(Node<Token> * root);

	private:
	
		Merger(const Merger &);
		Merger & operator=(const Merger &);
		static void	merge(Node<Token> *, Node<Token> *);

};

#endif