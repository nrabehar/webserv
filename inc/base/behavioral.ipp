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
		Element getState() const { return (_state); };
};

template <typename Element>
class IOriginator
{
	private:
		Element	_state;
	public:
		IOriginator(const Element & state) : _state(state) {};
		virtual ~IOriginator() {};
		void setState(const Element & state) { _state = state; };
		Element & getState() { return (_state); };
		const Element & getState() const { return (_state); };
		IMemento<Element>	save() const { return (IMemento<Element>(_state)); };
		void restore(const IMemento<Element> & m) { _state = m.getState(); };
};

template <typename Element>
class ICaretaker
{
	private:
		std::vector<IMemento<Element> >	_history;
	public:
		ICaretaker() {};
		virtual ~ICaretaker() {};
		void addMemento(const IMemento<Element> & m) { _history.push_back(m); };
		const IMemento<Element> & getMemento(size_t index) const
		{
			if (index >= _history.size())
				return (_history.back());
			return (_history[index]);
		};
		size_t getHistorySize() const { return (_history.size()); };
};

#endif
