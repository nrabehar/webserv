#ifndef IFACE_HPP
#define IFACE_HPP

class ISingleton
{
	public:
	virtual ~ISingleton() {};
	virtual bool isSingleton() const = 0;
};

template <typename T>
class IClonable
{
	public:
	virtual ~IClonable() {};
	virtual T * clone() const = 0;
};

class IPlaceholder :
public virtual ISingleton,
public virtual IClonable<IPlaceholder>
{
	public:
	virtual ~IPlaceholder() {};
	virtual bool isSingleton() const = 0;
	virtual IPlaceholder * clone() const = 0;
};

#endif
