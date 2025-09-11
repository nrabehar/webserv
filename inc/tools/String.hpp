#ifndef STRING_HPP
#define STRING_HPP

#include "../webserv.hpp"

class String
{

	private:


	public:
		String();
		~String();

		static std::string toLower(const std::string &);
		static std::string toUpper(const std::string &);
		static std::string trim(const std::string &);
		static std::string trim(const std::string &, const std::string &);
		static std::vector<std::string> split(const std::string &, const std::string &);

		static bool isNumber(const std::string &);

		static size_t toBytes(const std::string &);

	private:
		String(const String &);
		String & operator=(const String &);

};

#endif // STRING_HPP