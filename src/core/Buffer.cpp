#include "webserv.hpp"

Buffer::Buffer(): _rpos(0), _wpos(0) {}
Buffer::~Buffer() {}

const char * Buffer::readPtr() const
{

	if (!readable())
		return (NULL);

	return (_data.c_str() + _rpos);

}

size_t Buffer::readable() const
{

	if (_rpos >= _wpos)
		return (0);

	return (_wpos - _rpos);

}

void Buffer::hasRead(size_t n)
{

	if (n >= readable())
	{

		_rpos = 0;
		_wpos = 0;
		_data.clear();

	}
	else
	{

		_rpos += n;
		// ? if we have read a lot of data, we compact the buffer to avoid it growing too much
		compact();
	
	}

}

char * Buffer::writePtr()
{

	reserve(_wpos + 4096); // ? reserve at least 4KB more if needed
	return (const_cast<char *>(_data.c_str()) + _wpos);

}

size_t Buffer::writable() const
{

	if (_data.size() <= _wpos)
		return (0);

	return (_data.size() - _wpos);

}

void Buffer::hasWritten(size_t n)
{

	_wpos += n;
	if (_wpos > _data.size())
		_wpos = _data.size();

}

void Buffer::reserve(size_t size)
{

	if (_data.capacity() == 0)
		_data.reserve(4096);
	
	size_t new_size = _data.size();
	while (new_size < size)
		new_size = std::max(new_size * 2, new_size + 4096);
	_data.resize(new_size);

}

void Buffer::compact()
{

	if (_rpos == 0)
		return ;

	if (_rpos >= _wpos)
	{

		_rpos = 0;
		_wpos = 0;
		_data.clear();
		return ;

	}

	std::memmove(const_cast<char *>(_data.c_str()), _data.c_str() + _rpos, readable());
	_wpos -= _rpos;
	_rpos = 0;

}