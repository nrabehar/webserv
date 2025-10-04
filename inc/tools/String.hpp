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

		static std::string toLower(const std::string & s);
		static std::string toCamelCase(const std::string & s, char sep = '-');

		static bool	isNumeric(const std::string & s);

		template<typename T>
		static std::string str(const T & v)
		{

			std::ostringstream oss;
			oss << v;
			return (oss.str());

		}

	private:

		String();
		String(const String &);
		String & operator=(const String &);

};



#endif // STRING_HPP