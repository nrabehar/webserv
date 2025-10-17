#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

class Buffer
{

	private:

		std::vector<char> _data;
		size_t			_rpos;
		size_t			_wpos;

	public:

		Buffer();
		~Buffer();
		Buffer(const Buffer &);
		Buffer & operator=(const Buffer &);

		const char * readPtr() const;
		size_t readable() const;
		void hasRead(size_t n);

		char * writePtr();
		size_t writable() const;
		void hasWritten(size_t n);

		void	reserve(size_t size);

		void	clear();
		void  append(const std::string & str);
		void  append(const char * data, size_t len);

		size_t	find(const std::string & s) const;
		std::string substr(size_t start = 0, size_t n = std::string::npos) const;

	private:

		void	compact();

};


#endif