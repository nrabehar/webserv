#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

class IEventHandler
{

	public:

		virtual ~IEventHandler() {}
		virtual int	fd() = 0;

		virtual void	handle(short e) = 0;

};


#endif // EVENT_HANDLER_HPP