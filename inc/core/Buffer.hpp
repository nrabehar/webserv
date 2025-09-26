#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include <string>

#define BUFFER_SIZE	8192

class Buffer
{

	private:

		std::vector<char>	_data;
		size_t	_rpos;
		size_t	_wpos;

	public:

		Buffer();
		~Buffer();

	private:

		Buffer(const Buffer &);
		Buffer & operator=(const Buffer &);

		size_t	readable() const;
		const char *	rdPtr() const;
		void	hasRead(size_t n);
		size_t	writable() const;
		char *wrPtr();
		void	hasWritten(size_t n);
		void	append(const char * data, size_t n);
		void	append(const std::string & s);
		void	clear();

};


#endif // BUFFER_HPP