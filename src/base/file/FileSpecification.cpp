#include "webserv.hpp"

ReadableFile::ReadableFile() {}
ReadableFile::~ReadableFile() {}
bool ReadableFile::isSatisfiedBy(const IFile & file) const
{
	return (access(file.getPath().c_str(), R_OK) == 0);
}

WritableFile::WritableFile() {}
WritableFile::~WritableFile() {}
bool WritableFile::isSatisfiedBy(const IFile & file) const
{
	return (access(file.getPath().c_str(), W_OK) == 0);
}

ExecutableFile::ExecutableFile() {}
ExecutableFile::~ExecutableFile() {}
bool ExecutableFile::isSatisfiedBy(const IFile & file) const
{
	return (access(file.getPath().c_str(), X_OK) == 0);
}

RegularFile::RegularFile() {}
RegularFile::~RegularFile() {}
bool RegularFile::isSatisfiedBy(const IFile & file) const
{
	struct stat fileStat;
	if (stat(file.getPath().c_str(), &fileStat) != 0)
		return false;
	return S_ISREG(fileStat.st_mode);
}

DirectoryFile::DirectoryFile() {}
DirectoryFile::~DirectoryFile() {}
bool DirectoryFile::isSatisfiedBy(const IFile & file) const
{
	struct stat fileStat;
	if (stat(file.getPath().c_str(), &fileStat) != 0)
		return false;
	return S_ISDIR(fileStat.st_mode);
}

NonEmptyFile::NonEmptyFile() {}
NonEmptyFile::~NonEmptyFile() {}
bool NonEmptyFile::isSatisfiedBy(const IFile & file) const
{
	struct stat fileStat;
	if (stat(file.getPath().c_str(), &fileStat) != 0)
		return false;
	return fileStat.st_size > 0;
}
