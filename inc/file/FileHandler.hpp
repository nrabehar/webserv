#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include "../webserv.hpp"


class IFileHandler
{

	protected:

		typedef std::string FileReason;

	public:


		virtual ~IFileHandler() {};

		virtual IFileHandler* setNext(IFileHandler*) = 0;
		virtual IFileHandler* setPrev(IFileHandler*) = 0;
		virtual IFile *handle(const std::string& ) = 0;
		virtual void setReason(const FileReason &) = 0;
		virtual void	setFile(IFile *) = 0;
	
};

class FileHandler : public IFileHandler
{

	protected:

		IFileHandler*	_next;
		IFileHandler*	_prev;
		FileReason		_reason;

		IFile*				_file;

	public:

		FileHandler();
		virtual ~FileHandler();

		IFileHandler* setNext(IFileHandler*);
		IFileHandler* setPrev(IFileHandler*);
		void setReason(const FileReason &);

		IFile *handle(const std::string&);
		const FileReason& getReason() const;

	private:

		FileHandler(const FileHandler &);
		FileHandler &operator=(const FileHandler &);
		void	reset_chain();

	protected:

		void setFile(IFile *);

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

	private:

		int	_flags;

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