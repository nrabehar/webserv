#include "webserv.hpp"

Buffer::Buffer(): _rpos(0), _wpos(0) {}
Buffer::~Buffer() {}

Buffer::Buffer(const Buffer & other)
	: _data(other._data), _rpos(other._rpos), _wpos(other._wpos) {}

Buffer & Buffer::operator=(const Buffer & other)
{
	if (this != &other)
	{
		_data = other._data;
		_rpos = other._rpos;
		_wpos = other._wpos;
	}
	return (*this);
}

const char * Buffer::readPtr() const
{

	if (!readable())
		return (NULL);

	return (&_data[_rpos]);

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
		clear();

	else
	{

		_rpos += n;
		compact();
	
	}

}

char * Buffer::writePtr()
{

	if (writable() == 0)
		reserve(_wpos + 4096);
	return (&_data[_wpos]);

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
		return (clear());

	std::memmove(&_data[0], &_data[_rpos], readable());
	_wpos -= _rpos;
	_rpos = 0;

}

void	Buffer::clear()
{
	_rpos = 0;
	_wpos = 0;
	_data.clear();
}

void	Buffer::append(const std::string & str)
{

	size_t old_size = _data.size();
	_data.resize(old_size + str.size());
	std::memcpy(&_data[old_size], str.c_str(), str.size());
	_wpos += str.size();

}

size_t Buffer::find(const std::string & s) const
{
	if (!readable() || s.empty())
		return (std::string::npos);

	size_t readable_size = readable();
	const char * data = readPtr();

	for (size_t i = 0; i <= readable_size - s.size(); ++i)
	{
		if (i + s.size() > readable_size)
			break ;
		if (std::strncmp(&data[i], s.c_str(), s.size()) != 0)
			continue ;
		return (i);
	}

	return (std::string::npos);
}

std::string Buffer::substr(size_t start, size_t n) const
{
	size_t readable_size = readable();

	if (start >= readable_size)
		return ("");

	size_t len = std::min(n, readable_size - start);
	const char * data = readPtr() + start;

	return (std::string(data, len));
}
