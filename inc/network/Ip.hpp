#ifndef IP_HPP
#define IP_HPP

class Ip :
	public virtual IIp
{

	private:

		std::string			_host;
		int							_port;

	public:

		Ip();
		virtual ~Ip();
		Ip(const Ip &);
		Ip & operator=(const Ip &);

		void setHost(const std::string &);
		const std::string & getHost() const;
		void setPort(int);
		int getPort() const;

		virtual std::string str() const;

		virtual void clear();

		Ip(const ICopy &);
		Ip & operator=(const ICopy &);
		virtual void copy(const ICopy &);

		virtual IClone * clone() const;

		virtual void read(std::istream &);
		virtual void write(std::ostream &) const;

		virtual bool operator==(const IEqual &) const;
		virtual bool operator!=(const IEqual &) const;
		virtual bool operator<(const ICompare &) const;
		virtual bool operator<=(const ICompare &) const;
		virtual bool operator>(const ICompare &) const;
		virtual bool operator>=(const ICompare &) const;

		virtual bool valid() const;

		virtual void accept(const IGuest &) const;

};

#endif
