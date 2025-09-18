#ifndef FILE_IPP
#define FILE_IPP

#include "../../webserv.hpp"

class IFile
{
	public:

		virtual ~IFile() {}
		virtual int  getFD() const = 0;
		virtual bool isComplete() const = 0;
		virtual void read() = 0;
		virtual std::string getPath() const = 0;
		virtual std::string getData() const = 0;
		
};

class File : public IFile
{
	private:

		int 						_fd;
		bool						_complete;
		std::string					_data;
		std::string					_path;
	
	public:

		File(const std::string &);
		virtual ~File();

		virtual int  getFD() const;
		virtual bool isComplete() const;
		virtual void read();
		virtual std::string getPath() const;
		virtual std::string getData() const;

	private:

		File();
		File(const File &);
		File & operator=(const File &);

};

class FileProxy : public IFile
{
	private:

		IFile *		_file;
		std::string	_path;
	
	public:

		FileProxy(IFile *);
		virtual ~FileProxy();

		virtual int  getFD() const;
		virtual bool isComplete() const;
		virtual void read();
		virtual std::string getPath() const;
		virtual std::string getData() const;
	
	private:

		FileProxy();
		FileProxy(const FileProxy &);
		FileProxy & operator=(const FileProxy &);
};

#endif // FILE_IPP