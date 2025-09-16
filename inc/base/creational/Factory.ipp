#ifndef FACROTY_IPP
#define FACROTY_IPP 1

#include "../../webserv.hpp"

template <typename T>
class IFactory :
	public virtual ICreate<T>
{
	public:
		virtual ~IFactory() {};
};

template <typename T, typename U>
class Factory :
	public virtual IFactory<T>
{
	public:
		virtual ~Factory() {};
		virtual T *create() const { return (new U()); }
};

#endif /* FACROTY_IPP */
