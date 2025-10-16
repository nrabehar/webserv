#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

class IEventHandler
{

	public:

		virtual ~IEventHandler() {}
		virtual int	fd() const = 0;

		virtual void	handle(short e) = 0;
		virtual void	onTimeout() = 0;

};

class EventHandler: public IEventHandler
{

	protected:

		int _fd;
		int _timeout;
		time_t	_last_active;

	public:

		EventHandler();
		EventHandler(int fd = -1);
		~EventHandler();

		virtual int 	fd() const;
		virtual void	handle(short e) = 0;
		virtual void	onTimeout() = 0;

		time_t	lastActive() const;
		void	setLastActive(time_t t = time(NULL));
		void setTimeout(int timeout = 30);
		void reloadTimeout();

	private:

		EventHandler(const EventHandler &);
		EventHandler & operator=(const EventHandler &);

};



#endif