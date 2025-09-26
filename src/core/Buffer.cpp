#include "webserv.hpp"

Buffer::Buffer(): _rpos(0), _wpos(0) {}
Buffer::~Buffer() {}

size_t	Buffer::readable() const
{

	if (_wpos >= _rpos)
		return (_wpos - _rpos);
	return (0);

}

const char *	Buffer::rdPtr() const
{

	if (!readable())
		return (NULL);
	return (&_data[_rpos]);

}

void	Buffer::hasRead(size_t n)
{

	size_t	available = readable();
	if (n > available)
	{
		_rpos = 0;
		_wpos = 0;
	}
	else
	{
		_rpos += n;
	}

}