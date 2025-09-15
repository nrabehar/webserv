#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* Include files ***********************************************************  */

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <csignal>
#include <typeinfo>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

/* Macros ******************************************************************* */

#ifdef __GNUC__
	# define WRESULT __attribute__((warn_unused_result))
	# define PURE __attribute__((pure))
	# define NORETURN __attribute__((noreturn))
	# define NOTHROW __attribute__((nothrow))
#else
	# define WRESULT
	# define PURE
	# define NORETURN
	# define NOTHROW
#endif

/* Forward declarations ***************************************************** */

/* Enumerations ************************************************************* */

/* Type definitions ********************************************************* */

/* Interfaces *************************************************************** */

#include "data.ipp"
#include "data.hpp"

#include "network/Ip.ipp"

/* Base classes ************************************************************* */

#include "network/Ip.hpp"

/* Class includes *********************************************************** */

#endif // WEBSERV_HPP
