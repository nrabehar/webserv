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

/* Enumeration */

enum ECheck {
	CHK_OK,

	CHK_FILE_NOT_FOUND,
	CHK_FILE_NAME_EMPTY,
	CHK_FILE_NO_READ_PERMISSION,
	CHK_FILE_NO_WRITE_PERMISSION,
	CHK_FILE_NO_EXECUTE_PERMISSION,
	CHK_FILE_CONTENT_EMPTY,

};

/* Interfaces */

#include "interfaces.hpp"

/* Abstract classes */

#include "./tools/AFile.hpp"

/* Concrete classes */

#include "./conf/ConfigFile.hpp"
#include "./conf/Config.hpp"

#endif // WEBSERV_HPP
