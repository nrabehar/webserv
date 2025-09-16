#ifndef PROTOTYPE_IPP
#define PROTOTYPE_IPP 1

#include "../../webserv.hpp"

template <typename T>
class IPrototype :
	public virtual IClone<T>
{
	public: virtual ~IPrototype() {};
};

template <typename T, typename U>
class Prototype :
	public virtual IPrototype<T>
{
	public:
		virtual ~Prototype() {};
		virtual T * clone() const { return (new U(static_cast<const U &>(*this))); }
};

#endif /* PROTOTYPE_IPP */
