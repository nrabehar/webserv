#ifndef UTILS_HPP
#define UTILS_HPP

#include "../webserv.hpp"

class Utils
{
	public: virtual ~Utils();
	static std::string trim(
		const std::string & str,
		const std::string & delim = DEFAULT_WHITESPACE
	);
	static std::vector<std::string> split(
		const std::string & str,
		const std::string & delim = DEFAULT_DELIMITER
	);
	static std::string join(
		const std::vector<std::string> & vec,
		const std::string & delim = DEFAULT_WHITESPACE,
		const std::string & sep = ""
	);
	static std::string uncomment(
		const std::string & str,
		const std::string & symbol = "#"
	);
	static std::string sanitize(
		const std::string & str,
		const std::string & delim = DEFAULT_WHITESPACE
	);
	static bool braces(
		const std::string & str,
		const std::string & o = "{",
		const std::string & c = "}"
	);
	template<typename T>
	static std::string tostring(const T & value) {
		std::ostringstream oss; oss << value; return (oss.str());
	}
	template<typename T>
	static T fromstring(const std::string & str) {
		std::istringstream iss(str); T value; iss >> value; return (value);
	}
	private: Utils(); Utils(const Utils &); Utils & operator=(const Utils &);
};

#endif
