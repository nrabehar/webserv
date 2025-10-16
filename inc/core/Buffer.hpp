#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

/**
 * A simple buffer to manage read and write operations
 * without unnecessary data copying. This approach improves performance by allowing
 * direct access to the buffer's memory for reading and writing operations.
 * 
 * Note: We need to check char by char when parsing HTTP requests with this buffer
 */
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

		/**
		 * ? with readPtr() we can do write(buffer.readPtr(), buffer.size());
		 * ? so we read directly from the buffer without copying data (perf)
		 * ? and then we tell the buffer how many bytes we have read with hasRead(size);
		 */
		const char * readPtr() const;
		size_t readable() const;
		void hasRead(size_t n);

		/**
		 * ? with writePtr() we can do read(fd, buffer.writePtr(), buffer.capacity());
		 * ? so we read directly into the buffer without copying data (perf)
		 * ? and then we tell the buffer how many bytes we have written with hasWritten(size);
		 */
		char * writePtr();
		size_t writable() const;
		void hasWritten(size_t n);

		/**
		 * ? ensure there is enough space to write 'size' bytes
		 * ? if not, we resize the buffer
		 */
		void	reserve(size_t size);

		void	clear();
		void  append(const std::string & str);

		size_t	find(const std::string & s) const;
		std::string substr(size_t start = 0, size_t n = std::string::npos) const;

	private:

		/**
		 * ? if there is no more space to write, but there is space at the beginning
		 * ? (because we have read some data), we move the unread data to the beginning
		 * ? and update the read and write positions
		 */
		void	compact();

};


#endif