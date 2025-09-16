#ifndef VISITOR_HPP
#define VISITOR_HPP 1

#include "../../webserv.hpp"

template <typename T>
class IGuest
{
	public:
		virtual ~IGuest() {}
		virtual void visit(T &) = 0;
		virtual void visit(const T &) = 0;
};

template <typename T>
class Guest :
	public virtual IGuest<T>
{
	public:
		virtual ~Guest() {}
		virtual void visit(T &) {}
		virtual void visit(const T &) {}
};

template <typename T>
class IHost
{
	public:
		virtual ~IHost() {}
		virtual void accept(IGuest<T> &) = 0;
};

template <typename T, typename U>
class Host :
	public virtual IHost<T>
{
	public:
		virtual ~Host() {}
		virtual void accept(IGuest<T> & visitor, U * ptr) {
							    visitor.visit(*ptr);
		}
};

#endif /* VISITOR_HPP */
