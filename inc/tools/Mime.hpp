#ifndef MIME_HPP
#define MIME_HPP


#include "../webserv.hpp"

class Mime
{

	private:

		static std::map<std::string, std::string>	_types;

	public:

		~Mime();

		static std::string getType(const std::string & filename);

	private:

		Mime(const Mime &);
		Mime & operator=(const Mime &);
		Mime();

};



#endif // MIME_HPP