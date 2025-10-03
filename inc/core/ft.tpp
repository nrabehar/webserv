#ifndef FT_TPP
#define FT_TPP

#include <new>
#include <cstddef>

#include "core/mid.hpp"

template <typename T>
NOTHROW WRESULT
ifaceof(T) * ft::null()
{
	return (ft_type<ifaceof(T)>::nil::instance());
}

#undef nilof
#define nilof(T) ft::null<T>()

template <typename T>
NOTHROW WRESULT
ifaceof(T) * ft::alloc()
{
	_ft_static_assert(!ft_type<T>::is_nilobj);
	if (ft::_simulate_failure)
	{
		return (nilof(T));
	}
	T * o = new (std::nothrow) T();
	if (!o)
	{
		return (nilof(T));
	}
	return (o);
}

template <typename T, typename P1>
NOTHROW WRESULT
ifaceof(T) * ft::alloc(P1 p1)
{
	_ft_static_assert(!ft_type<T>::is_nilobj);
	if (ft::_simulate_failure)
	{
		return (nilof(T));
	}
	T * o = new (std::nothrow) T(p1);
	if (!o)
	{
		return (nilof(T));
	}
	return (o);
}

template <typename T, typename P1, typename P2>
NOTHROW WRESULT
ifaceof(T) * ft::alloc(P1 p1, P2 p2)
{
	_ft_static_assert(!ft_type<T>::is_nilobj);
	if (ft::_simulate_failure)
	{
		return (nilof(T));
	}
	T * o = new (std::nothrow) T(p1, p2);
	if (!o)
	{
		return (nilof(T));
	}
	return (o);
}

template <typename T, typename P1, typename P2, typename P3>
NOTHROW WRESULT
ifaceof(T) * ft::alloc(P1 p1, P2 p2, P3 p3)
{
	_ft_static_assert(!ft_type<T>::is_nilobj);
	if (ft::_simulate_failure)
	{
		return (nilof(T));
	}
	T * o = new (std::nothrow) T(p1, p2, p3);
	if (!o)
	{
		return (nilof(T));
	}
	return (o);
}

template <typename T>
NOTHROW
void ft::free(T *& o)
{
	if (o != nilof(T))
	{
		delete o;
	}
	o = nilof(T);
}

template <typename T>
Holder<T>::~Holder()
{
}

template <typename T>
Holder<T>::Holder(const T & obj) : _ptr(obj)
{
}

template <typename T>
ifaceof(T) * Holder<T>::get()
{
	return (&_ptr);
}

template <typename T>
const ifaceof(T) * Holder<T>::get() const
{
	return (&_ptr);
}

template <typename T>
IPlaceholder * Holder<T>::clone() const
{
	return (ft::alloc<Holder<T> >(_ptr));
}

template <typename T>
Holder<T>::Holder() : _ptr(nilof(T))
{
}

template <typename T>
Holder<T>::Holder(const Holder & src) : _ptr(nilof(T))
{
	(*this) = src;
}

template <typename T>
Holder<T> & Holder<T>::operator=(const Holder & rhs)
{
	if (this != &rhs)
	{
		ft::free(_ptr);
		if (rhs._ptr != nilof(T))
		{
			_ptr = ft::alloc<T>(*(rhs._ptr));
		}
	}
	return (*this);
}

template <typename T>
NilHolder<T>::~NilHolder()
{
}

template <typename T>
IHolder<T> * NilHolder<T>::instance()
{
	static NilHolder<T> inst;
	return (&inst);
}

template <typename T>
ifaceof(T) * NilHolder<T>::get()
{
	return (nilof(T));
}

template <typename T>
const ifaceof(T) * NilHolder<T>::get() const
{
	return (nilof(T));
}

template <typename T>
IPlaceholder * NilHolder<T>::clone() const
{
	return (NilHolder<T>::instance());
}

template <typename T>
NilHolder<T>::NilHolder()
{
}

template <typename T>
NilHolder<T>::NilHolder(const NilHolder & src)
{
	(void)src;
}

template <typename T>
NilHolder<T> & NilHolder<T>::operator=(const NilHolder & rhs)
{
	if (this != &rhs)
	{
	}
	return (*this);
}

template <typename T>
ft::any::any(const T & value) : _holder(NULL)
{
	_holder = ft::alloc<Holder<T> >(value);
}

template <typename T>
ifaceof(T) * ft::any::get()
{
	IHolder<T> * h = dynamic_cast<IHolder<T> *>(_holder);
	if (h)
	{
		return (h->get());
	}
	return (nilof(T));
}

template <typename T>
const ifaceof(T) * ft::any::get() const
{
	const IHolder<T> * h = dynamic_cast<const IHolder<T> *>(_holder);
	if (h)
	{
		return (h->get());
	}
	return (nilof(T));
}

#endif
