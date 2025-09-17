#ifndef INMEMORYFILE_HPP
#define INMEMORYFILE_HPP

#include "../../webserv.hpp"

class InMemoryFile: public File
{

	private:


	public:

		InMemoryFile(const std::string &);
		virtual ~InMemoryFile();

		virtual bool open();
		virtual void close();
		virtual size_t read(char *, size_t);
		virtual size_t write(const char *, size_t);
		virtual size_t size() const;

	private:

		InMemoryFile();
		InMemoryFile(const InMemoryFile &);
		InMemoryFile & operator=(const InMemoryFile &);

};

#endif // INMEMORYFILE_HPP