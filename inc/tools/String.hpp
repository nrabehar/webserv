#ifndef STRING_HPP
#define STRING_HPP

#include "../webserv.hpp"


class String
{

	private:

		

	public:

		~String();

		static std::string trim(const std::string & s, const std::string & delim);
		static std::string trim(const std::string & s);

		static std::vector<std::string>	split(const std::string & s, const std::string & delim);
		static std::vector<std::string> split(const std::string & s);

		static bool	isNumeric(const std::string & s);

	private:

		String();
		String(const String &);
		String & operator=(const String &);

};



#endif // STRING_HPP