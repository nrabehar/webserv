#ifndef CONFIGNODE_HPP
#define CONFIGNODE_HPP

#include "../webserv.hpp"

class ConfigNode
{

	private:


		std::string								_name;
		std::vector<std::string>	_args;
		std::vector<ConfigNode *> _child;
		

	public:

		ConfigNode(const std::string &);
		ConfigNode(const ConfigNode &);
		~ConfigNode();

		void	pushArg(const std::string &);
		void	addChild(ConfigNode *);

		const std::string&	getName() const;
		const std::vector<std::string>&	getArg() const;
		std::vector<ConfigNode *>& getChild();

		void	print(size_t) const;

	private:

		ConfigNode();
		ConfigNode & operator=(const ConfigNode &);

};

#endif // CONFIGNODE_HPP