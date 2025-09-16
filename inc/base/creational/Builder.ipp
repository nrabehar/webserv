#ifndef BUILDER_IPP
#define BUILDER_IPP 1

#include "../../webserv.hpp"

template <typename T>
class IBuilder :
	public virtual IReset,
	public virtual IBuild,
	public virtual IGetPtr<T>
{
	public:
		virtual ~IBuilder() {};
};

template <typename T, typename U>
class Builder :
	public virtual IBuilder<T>
{
	private:
		T *_data;
	public:
		virtual ~Builder() { delete _data; }
		virtual void build() { _data = new U(); }
		virtual T *get() { return (_data); }
		virtual void reset() { _data = NULL; }
};

#endif /* BUILDER_IPP */
