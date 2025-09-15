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

		// Setters/Getters
		void setHost(const std::string &);
		const std::string & getHost() const;
		void setPort(int);
		int getPort() const;

		// IData
		virtual std::string str() const;

		// IClear
		virtual void clear();

		// ICopy
		Ip(const ICopy &);
		Ip & operator=(const ICopy &);
		virtual void copy(const ICopy &);

		// IClone
		virtual IClone * clone() const;

		// IIO
		virtual void read(std::istream &);
		virtual void write(std::ostream &) const;

		// ICompare
		virtual bool operator==(const IEqual &) const;
		virtual bool operator!=(const IEqual &) const;
		virtual bool operator<(const ICompare &) const;
		virtual bool operator<=(const ICompare &) const;
		virtual bool operator>(const ICompare &) const;
		virtual bool operator>=(const ICompare &) const;

		// IValid
		virtual bool valid() const;

	protected:

		virtual std::string _extractHost(const std::string &);
		virtual int _extractPort(const std::string &);
		virtual bool _isValidHost(const std::string &) const;
		virtual bool _isValidPort(int) const;

};

#endif // IP_HPP
