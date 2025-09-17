#ifndef FILE_IPP
#define FILE_IPP

#include "../../webserv.hpp"

class IFile
{
	public:

		virtual ~IFile() {}
		virtual bool open() = 0;
		virtual void close() = 0;
		virtual size_t read(char *, size_t) = 0;
		virtual size_t write(const char *, size_t) = 0;
		virtual size_t size() const = 0;

};

class File : public IFile
{

	protected:

		int	_fd;
		std::string	_path;


	public:

		File(const std::string &);
		virtual ~File();

		const std::string & getPath() const;

	private:

		File();
		File(const File &);
		File & operator=(const File &);

};

class FileFactory
{

	private:



	public:

		~FileFactory();

		static IFile * create(const std::string &);

	private:

		FileFactory();
		FileFactory(const FileFactory &);
		FileFactory & operator=(const FileFactory &);

};



#endif // FILE_IPP