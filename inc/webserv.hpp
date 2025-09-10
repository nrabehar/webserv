#ifndef WEBSERV_HPP
#define WEBSERV_HPP 1

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <ctime>
#include <fstream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <exception>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* Forward declarations */

class AFile;

/* Enumeration */

enum EValidator {
	VALIDATOR_OK,
	VALIDATOR_FILE_NOT_FOUND,
	VALIDATOR_FILE_NAME_EMPTY,
	VALIDATOR_FILE_NO_READ_PERMISSION,
	VALIDATOR_FILE_NO_WRITE_PERMISSION,
	VALIDATOR_FILE_NO_EXECUTE_PERMISSION,
	VALIDATOR_FILE_CONTENT_EMPTY,
};

/* Interfaces */

class IValidator
{
	public:
		virtual ~IValidator() {};
		virtual EValidator validate() const = 0;
};

class IParser
{
	public:
		virtual ~IParser() {};
		virtual bool parse(const std::string & = "") = 0;
};

class ILoader
{
	public:
		virtual ~ILoader() {};
		virtual bool load() = 0;
};

class IHandler
{
	public:
		virtual ~IHandler() {};
		virtual bool handle() = 0;
};

/* Abstract classes */

#include "./tools/AFile.hpp"

/* Concrete classes */

#include "./conf/ConfigFile.hpp"
#include "./conf/Config.hpp"

#endif // WEBSERV_HPP
