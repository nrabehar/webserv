#include "core/ft.hpp"

ft::any::~any()
{
	if (_holder && !_holder->isSingleton())
	{
		delete _holder;
		_holder = NULL;
	}
}

ft::any::any() : _holder(NULL)
{
}

ft::any::any(const ft::any & src) : _holder(NULL)
{
	(*this) = src;
}

ft::any & ft::any::operator=(const ft::any & rhs)
{
	if (this != &rhs)
	{
		if (_holder && !_holder->isSingleton())
		{
			delete _holder;
			_holder = NULL;
		}
		if (rhs._holder)
		{
			_holder = rhs._holder->clone();
		}
	}
	return (*this);
}
