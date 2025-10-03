#ifndef FT_IPP
#define FT_IPP

#include <cstddef>

#include "base/iface.hpp"

template <bool> struct ft_assert;
template <> struct ft_assert<true> {};

#undef _ft_static_assert
#define _ft_static_assert(expr) (void)sizeof(ft_assert<(expr)>)

template <typename T> struct nil_default
{
	static T * instance()
	{
		return (NULL);
	}
};

template <typename T> struct ft_type
{
	enum
	{
		is_custom = false,
		is_nilobj = false,
		is_interface = false,
		has_interface = false,
		has_nilobj = false,
	};
	typedef nil_default<T> nil;
	typedef T iface;
};

#undef ifaceof
#define ifaceof(T) typename ft_type<T>::iface

#undef __register_itype
#define __register_itype(IFace, Type) \
template <> struct ft_type<Type> \
{ \
	enum \
	{ \
		is_custom = true, \
		is_nilobj = false, \
		is_interface = false, \
		has_interface = true, \
		has_nilobj = false, \
	}; \
	typedef Type nil; \
	typedef IFace iface; \
};

#undef __register_inilobj
#define __register_inilobj(IFace, NilObj) \
template <> struct ft_type<IFace> \
{ \
	enum \
	{ \
		is_custom = true, \
		is_nilobj = false, \
		is_interface = true, \
		has_interface = true, \
		has_nilobj = true, \
	}; \
	typedef NilObj nil; \
	typedef IFace iface; \
}; \
\
template <> struct ft_type<NilObj> \
{ \
	enum \
	{ \
		is_custom = true, \
		is_nilobj = true, \
		is_interface = false, \
		has_interface = true, \
		has_nilobj = true, \
	}; \
	typedef NilObj nil; \
	typedef IFace iface; \
};

#undef __register_itype_template
#define __register_itype_template(IFace, Type) \
template <typename T> struct ft_type<Type<T> > \
{ \
	enum \
	{ \
		is_custom = true, \
		is_nilobj = false, \
		is_interface = false, \
		has_interface = true, \
		has_nilobj = false, \
	}; \
	typedef Type<T> nil; \
	typedef IFace<T> iface; \
};

#undef __register_inilobj_template
#define __register_inilobj_template(IFace, NilObj) \
template <typename T> struct ft_type<IFace<T> > \
{ \
	enum \
	{ \
		is_custom = true, \
		is_nilobj = false, \
		is_interface = true, \
		has_interface = true, \
		has_nilobj = true, \
	}; \
	typedef NilObj<T> nil; \
	typedef IFace<T> iface; \
}; \
\
template <typename T> struct ft_type<NilObj<T> > \
{ \
	enum \
	{ \
		is_custom = true, \
		is_nilobj = true, \
		is_interface = false, \
		has_interface = true, \
		has_nilobj = true, \
	}; \
	typedef NilObj<T> nil; \
	typedef IFace<T> iface; \
};

#endif
