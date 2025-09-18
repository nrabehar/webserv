#ifndef FILESPECIFICATION_HPP
#define FILESPECIFICATION_HPP

#include "../../webserv.hpp"

class IFileSpecification
{

	public:

		virtual ~IFileSpecification() {}
		virtual bool isSatisfiedBy(const IFile & file) const = 0;

};

class ReadableFile: public IFileSpecification
{

	public:

		ReadableFile();
		virtual ~ReadableFile();
		virtual bool isSatisfiedBy(const IFile & file) const;

	private:

		ReadableFile(const ReadableFile &);
		ReadableFile & operator=(const ReadableFile &);

};

class WritableFile: public IFileSpecification
{

	public:

		WritableFile();
		virtual ~WritableFile();
		virtual bool isSatisfiedBy(const IFile & file) const;

	private:

		WritableFile(const WritableFile &);
		WritableFile & operator=(const WritableFile &);

};

class ExecutableFile: public IFileSpecification
{

	public:

		ExecutableFile();
		virtual ~ExecutableFile();
		virtual bool isSatisfiedBy(const IFile & file) const;

	private:

		ExecutableFile(const ExecutableFile &);
		ExecutableFile & operator=(const ExecutableFile &);

};

class RegularFile: public IFileSpecification
{

	public:

		RegularFile();
		virtual ~RegularFile();
		virtual bool isSatisfiedBy(const IFile & file) const;

	private:

		RegularFile(const RegularFile &);
		RegularFile & operator=(const RegularFile &);

};

class DirectoryFile: public IFileSpecification
{

	public:

		DirectoryFile();
		virtual ~DirectoryFile();
		virtual bool isSatisfiedBy(const IFile & file) const;

	private:

		DirectoryFile(const DirectoryFile &);
		DirectoryFile & operator=(const DirectoryFile &);

};

class NonEmptyFile: public IFileSpecification
{

	public:

		NonEmptyFile();
		virtual ~NonEmptyFile();
		virtual bool isSatisfiedBy(const IFile & file) const;

	private:

		NonEmptyFile(const NonEmptyFile &);
		NonEmptyFile & operator=(const NonEmptyFile &);

};

#endif // FILESPECIFICATION_HPP