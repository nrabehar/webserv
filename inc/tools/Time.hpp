#ifndef TIME_HPP
#define TIME_HPP

#include <ctime>
#include <string>

class Time
{

	private:

		

	public:

		Time();
		~Time();
		static std::time_t	now();
		static std::string  timeToStr(std::time_t t = now(), const std::string & format = "%a %b %d %H:%M:%S %Y");
		static long diff(std::time_t start, std::time_t end);
		static std::time_t toTime(const std::string &str);
		static std::time_t toTime(int t);
		static std::string timeToHttpStr(std::time_t t = now());

	private:

		Time(const Time &);
		Time & operator=(const Time &);

};


#endif // TIME_HPP