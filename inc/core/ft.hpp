#ifndef FT_HPP
#define FT_HPP

#include "base/attr.hpp"
#include "core/ft.ipp"

template <typename T>
class IHolder : public IPlaceholder
{
	public: virtual ~IHolder() {};
	virtual ifaceof(T) * get() = 0;
	virtual const ifaceof(T) * get() const = 0;
	virtual IPlaceholder * clone() const = 0;
	virtual bool isSingleton() const = 0;
};

template <typename T>
class Holder : public IHolder<T>
{
	private:
	T _ptr;
	public:
	virtual ~Holder();
	Holder(const T & obj);
	virtual ifaceof(T) * get();
	virtual const ifaceof(T) * get() const;
	virtual IPlaceholder * clone() const;
	virtual bool isSingleton() const { return false; }
	private:
	Holder();
	Holder(const Holder & src);
	Holder & operator=(const Holder & rhs);
};

__register_itype_template(IHolder, Holder);

template <typename T>
class NilHolder : public IHolder<T>
{
	public:
	virtual ~NilHolder();
	static IHolder<T> * instance();
	virtual ifaceof(T) * get();
	virtual const ifaceof(T) * get() const;
	virtual IPlaceholder * clone() const;
	virtual bool isSingleton() const { return true; }
	private:
	NilHolder();
	NilHolder(const NilHolder & src);
	NilHolder & operator=(const NilHolder & rhs);
};

__register_inilobj_template(IHolder, NilHolder);

struct ft
{
	template <typename T>
		static ifaceof(T) * null() NOTHROW WRESULT;
	template <typename T>
		static ifaceof(T) * alloc() NOTHROW WRESULT;
	template <typename T, typename P1>
		static ifaceof(T) * alloc(P1) NOTHROW WRESULT;
	template <typename T, typename P1, typename P2>
		static ifaceof(T) * alloc(P1, P2) NOTHROW WRESULT;
	template <typename T, typename P1, typename P2, typename P3>
		static ifaceof(T) * alloc(P1, P2, P3) NOTHROW WRESULT;
	template <typename T>
		static void free(T *& o) NOTHROW;
	struct any
	{
		private: IPlaceholder * _holder;
		public: virtual ~any();
		any(); any(const any & src); any & operator=(const any & rhs);
		template <typename T> any(const T & value);
		template <typename T> ifaceof(T) * get();
		template <typename T> const ifaceof(T) * get() const;
	};
	static void failure(bool value) NOTHROW;
	private:
	static bool _simulate_failure;
};

#include "core/mid.hpp"

#endif
