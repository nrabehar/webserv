#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

class IEventHandler
{

	public:

		virtual ~IEventHandler() {}
		virtual int	fd() const = 0;

		virtual void	handle(short e) = 0;

};

class EventHandler: public IEventHandler
{

	protected:

		int _fd;

	public:

		EventHandler();
		EventHandler(int fd = -1);
		~EventHandler();

		virtual int 	fd() const;
		virtual void	handle(short e) = 0;

	private:

		EventHandler(const EventHandler &);
		EventHandler & operator=(const EventHandler &);

};



#endif // EVENT_HANDLER_HPP