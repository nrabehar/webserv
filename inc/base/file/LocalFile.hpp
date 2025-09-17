#ifndef LOCALFILE_HPP
#define LOCALFILE_HPP

#include "../../webserv.hpp"

class LocalFile : public File
{

	public:

		LocalFile(const std::string &);
		virtual ~LocalFile();

		virtual bool open();
		virtual void close();
		virtual size_t read(char *, size_t);
		virtual size_t write(const char *, size_t);
		virtual size_t size() const;

	private:

		LocalFile();
		LocalFile(const LocalFile &);
		LocalFile & operator=(const LocalFile &);

};

#endif // LOCALFILE_HPP