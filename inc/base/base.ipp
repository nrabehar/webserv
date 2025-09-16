#ifndef BASE_IPP
#define BASE_IPP 1

#include "../webserv.hpp"

class IClear
{
	public:
		virtual ~IClear() {}
		virtual void clear() = 0;
};

template <typename T>
class ICreate
{
	public:
		virtual ~ICreate() {}
		virtual T * create() const = 0;
};

class IBuild
{
	public:
		virtual ~IBuild() {}
		virtual void build() = 0;
};

template <typename T>
class ICopy
{
	public:
		virtual ~ICopy() {}
		virtual void copy(const T &) = 0;
};

template <typename T>
class IClone
{
	public:
		virtual ~IClone() {}
		virtual T * clone() const = 0;
};

class IReset
{
	public:
		virtual ~IReset() {}
		virtual void reset() = 0;
};

template <typename T>
class IGetPtr
{
	public:
		virtual ~IGetPtr() {}
		virtual T * get() const = 0;
};

template <typename T>
class IGetRef
{
	public:
		virtual ~IGetRef() {}
		virtual T & get() const = 0;
};

#endif /* BASE_IPP */
