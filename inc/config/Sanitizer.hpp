#ifndef SANITIZER_HPP
#define SANITIZER_HPP

#include "../webserv.hpp"

class Config::Sanitizer
{

	public:

		Sanitizer();
		~Sanitizer();

		static std::string sanitize(const std::string & s);

	private:

		Sanitizer(const Sanitizer &);
		Sanitizer & operator=(const Sanitizer &);

		static std::string removeComments(const std::string & s);
		static std::string normalizeNewlines(const std::string & s);
		static std::string normalizeBrackets(const std::string & s);
		
};

#endif