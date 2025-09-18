#ifndef BEHAVIORAL_IPP
#define BEHAVIORAL_IPP

#include "../webserv.hpp"

template <typename GuestType, typename HostType>
class IGuest
{

	public: virtual ~IGuest() {};

	virtual void visit(IHost<HostType, GuestType> &) = 0;

};

template <typename HostType, typename GuestType>
class IHost
{

	public: virtual ~IHost() {};

	virtual void accept(IGuest<GuestType, HostType> & g)
	{
		if (dynamic_cast<GuestType *>(&g))
			g.visit(*this);
	}

};

template <typename Element>
class IMemento
{

	private:

		Element	_state;

	public:

		IMemento(const Element & state) : _state(state) {};
		virtual ~IMemento() {};

		Element get() const { return (_state); };

};

template <typename Element>
class IOriginator
{

	private:

		Element	_state;

	public:

		IOriginator(const Element & state) : _state(state) {};
		virtual ~IOriginator() {};

		void set(const Element & state) { _state = state; };

		Element & get() { return (_state); };
		const Element & get() const { return (_state); };

		IMemento<Element>	save() const { return (IMemento<Element>(_state)); };

		void restore(const IMemento<Element> & m) { _state = m.get(); };

};

template <typename Element>
class ICaretaker
{

	private:

		std::vector<IMemento<Element> >	_history;

	public:

		virtual ~ICaretaker() {};

		void add(const IMemento<Element> & m) { _history.push_back(m); };

		const IMemento<Element> & get(size_t index) const
		{
			if (index >= _history.size()) return (_history.back());
			return (_history[index]);
		};

		size_t size() const { return (_history.size()); };

};

#endif
