#ifndef CONFIGMERGER_HPP
#define CONFIGMERGER_HPP

#include "../webserv.hpp"

class ConfigMerger
{

	private:
		

	public:
		
		ConfigMerger();
		~ConfigMerger();

		static void	merge(Node<std::string> *, Node<std::string> *);

	private:
	
		ConfigMerger(const ConfigMerger &);
		ConfigMerger & operator=(const ConfigMerger &);

};

#endif // CONFIGMERGER_HPP