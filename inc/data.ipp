#ifndef DATA_IPP
#define DATA_IPP

class IData {
	public:
		virtual ~IData() {};
		virtual std::string str() const = 0;
};

class IClear :
	public virtual IData
{
	public:
		virtual ~IClear() {};
		virtual void clear() = 0;
};

class IOutput :
	public virtual IData
{
	public:
		virtual ~IOutput() {};
		virtual void write(std::ostream &) const = 0;
};

class IInput :
	public virtual IData
{
	public:
		virtual ~IInput() {};
		virtual void read(std::istream &) = 0;
};

class IIO :
	public virtual IData,
	public virtual IInput,
	public virtual IOutput
{
	public:
		virtual ~IIO() {};
};

class IWrite :
	public virtual IData
{
	public:
		virtual ~IWrite() {};
		virtual void write() const = 0;
};

class IRead :
	public virtual IData
{
	public:
		virtual ~IRead() {};
		virtual void read() = 0;
};

class IParse :
	public virtual IData
{
	public:
		virtual ~IParse() {};
		virtual void parse() const = 0;
};

class IOpen :
	public virtual IData
{
	public:
		virtual ~IOpen() {};
		virtual void open() = 0;
};

class IClose :
	public virtual IData
{
	public:
		virtual ~IClose() {};
		virtual void close() = 0;
};

class ICopy :
	public virtual IData
{
	public:
		virtual ~ICopy() {};
		virtual void copy(const ICopy &) = 0;
};

class IClone :
	public virtual IData
{
	public:
		virtual ~IClone() {};
		virtual IClone * clone() const = 0;
};

class IEqual :
	public virtual IData
{
	public:
		virtual ~IEqual() {};
		virtual bool operator==(const IEqual &) const = 0;
		virtual bool operator!=(const IEqual &) const = 0;
};

class ICompare :
	public virtual IEqual
{
	public:
		virtual ~ICompare() {};
		virtual bool operator==(const IEqual &) const = 0;
		virtual bool operator!=(const IEqual &) const = 0;
		virtual bool operator<(const ICompare &) const = 0;
		virtual bool operator<=(const ICompare &) const = 0;
		virtual bool operator>(const ICompare &) const = 0;
		virtual bool operator>=(const ICompare &) const = 0;
};

class IValid :
	public virtual IData
{
	public:
		virtual ~IValid() {};
		virtual bool valid() const = 0;
};

class IGuest :
	public virtual IData
{
	public:
		virtual ~IGuest() {};
		virtual void visit(const IData &) const = 0;
};

class IHost :
	public virtual IData
{
	public:
		virtual ~IHost() {};
		virtual void accept(const IGuest &) const = 0;
};

std::ostream & operator<<(std::ostream &, const IOutput &);
std::istream & operator>>(std::istream &, IInput &);

#endif
