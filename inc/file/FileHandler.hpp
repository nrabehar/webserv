#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include "../webserv.hpp"


class IFileHandler
{

	public:

		virtual ~IFileHandler() = default;

		virtual IFileHandler* setNext(IFileHandler*) = 0;
		virtual IFile *handle(const std::string& ) = 0;
	
};

class FileHandler : public IFileHandler
{

	private:

		typedef std::string FileReason;

		IFileHandler*	_next;
		FileReason		_reason;

	public:

		FileHandler();
		virtual ~FileHandler() {}

		IFileHandler* setNext(IFileHandler*);

		IFile *handle(const std::string&);
		const FileReason& getReason() const;

	private:

		FileHandler(const FileHandler &);
		FileHandler &operator=(const FileHandler &);

	protected:

		void setReason(const FileReason &);

};

class FilePathBuilder : public FileHandler
{

	private:

		std::string	_root;
		std::string _uri;

	public:

		FilePathBuilder(const std::string&, const std::string &);
		virtual ~FilePathBuilder();

		IFile *handle(const std::string&);

	private:

		FilePathBuilder();
		FilePathBuilder(const FilePathBuilder &);
		FilePathBuilder & operator=(const FilePathBuilder &);

};

class FileExistenceChecker : public FileHandler
{

	public:

		FileExistenceChecker();
		virtual ~FileExistenceChecker();

		IFile *handle(const std::string&);

	private:

		FileExistenceChecker(const FileExistenceChecker &);
		FileExistenceChecker & operator=(const FileExistenceChecker &);

};

class FilePermissionChecker : public FileHandler
{

	public:

		FilePermissionChecker(int);
		virtual ~FilePermissionChecker();

		IFile *handle(const std::string&);

	private:

		FilePermissionChecker(const FilePermissionChecker &);
		FilePermissionChecker & operator=(const FilePermissionChecker &);

};

class FileOpener : public FileHandler
{

	public:

		FileOpener();
		virtual ~FileOpener();

		IFile *handle(const std::string&);

	private:

		FileOpener(const FileOpener &);
		FileOpener & operator=(const FileOpener &);

};

	

#endif // FILEHANDLER_HPP