#ifndef OBSERVER_HPP
#define OBSERVER_HPP


#include "../../webserv.hpp"
#include <vector>
#include <map>
#include <algorithm>


enum EventType {
};

template <typename T>
class IObserver
{
	public:

		virtual ~IObserver() {}
		virtual void update(T const &) = 0;

};

template <typename T>
class Observable
{

	private:

		std::map<EventType, std::vector<IObserver<T> *> > _observers;

	public:

		virtual ~Observable() {}

		   void attach(EventType eventType, IObserver<T> * observer) {
			   this->_observers[eventType].push_back(observer);
		   }

		   void detach(EventType eventType, IObserver<T> * observer) {
			   std::vector<IObserver<T> *> & observers = this->_observers[eventType];
			   typename  std::vector<IObserver<T> *>::iterator it;

				 it = std::find(observers.begin(), observers.end(), observer);
			   if (it != observers.end())
				   observers.erase(it);
		   }

		   void notify(EventType eventType, T const & event) {
			   typename std::map<EventType, std::vector<IObserver<T> *> >::iterator oit;

			   oit = this->_observers.find(eventType);
			   if (oit == this->_observers.end())
				   return;
			   std::vector<IObserver<T> *> & observers = oit->second;
			   typename std::vector<IObserver<T> *>::iterator it;
			   for (it = observers.begin(); it != observers.end(); ++it) {
				   (*it)->update(event);
			   }
		   }
};

#endif // OBSERVER_HPP