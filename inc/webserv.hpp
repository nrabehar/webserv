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
#include <utility>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* Enumeration */

enum EStatusCode {
	ST_OK = 200,
	ST_CREATED = 201,
	ST_ACCEPTED = 202,
	ST_NO_CONTENT = 204,
	ST_MOVED_PERMANENTLY = 301,
	ST_FOUND = 302,
	ST_SEE_OTHER = 303,
	ST_NOT_MODIFIED = 304,
	ST_BAD_REQUEST = 400,
	ST_UNAUTHORIZED = 401,
	ST_FORBIDDEN = 403,
	ST_NOT_FOUND = 404,
	ST_METHOD_NOT_ALLOWED = 405,
	ST_INTERNAL_SERVER_ERROR = 500,
	ST_NOT_IMPLEMENTED = 501,
	ST_BAD_GATEWAY = 502,
	ST_SERVICE_UNAVAILABLE = 503,

	ST_NO_READ_PERMISSION = 99,
	ST_NO_WRITE_PERMISSION = 98,
	ST_NO_EXECUTE_PERMISSION = 97,
	ST_EMPTY_NAME = 96,
	ST_EMPTY_CONTENT = 95,
};

typedef EStatusCode ECheck;
typedef EStatusCode EErrorCode;
typedef EStatusCode EFileStatus;

/* Interfaces */

#include "interfaces.hpp"

/* Abstract classes */

#include "./tools/AFile.hpp"
#include "./tools/String.hpp"
#include "./tools/Block.hpp"

/* Concrete classes */

#include "./server/Location.hpp"
#include "./server/Server.hpp"

#include "./conf/ConfigParser.hpp"
#include "./conf/ConfigFile.hpp"
#include "./conf/Config.hpp"

#endif // WEBSERV_HPP
