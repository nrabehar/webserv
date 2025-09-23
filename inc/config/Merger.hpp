#ifndef CONFIGMERGER_HPP
#define CONFIGMERGER_HPP

#include "../webserv.hpp"

class Config::Merger
{

	public:
		
		Merger();
		~Merger();

		static void	merge(Node<Token> *, Node<Token> *);

	private:
	
		Merger(const Merger &);
		Merger & operator=(const Merger &);

};

#endif // CONFIGMERGER_HPP