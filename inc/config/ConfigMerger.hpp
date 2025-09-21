#ifndef CONFIGMERGER_HPP
#define CONFIGMERGER_HPP

#include "../webserv.hpp"

class ConfigMerger
{

	private:
		

	public:
		
		ConfigMerger();
		~ConfigMerger();

		static void	merge(Node<Token> *, Node<Token> *);

	private:
	
		ConfigMerger(const ConfigMerger &);
		ConfigMerger & operator=(const ConfigMerger &);

};

#endif // CONFIGMERGER_HPP