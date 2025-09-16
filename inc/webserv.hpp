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

#undef DEFAULT_CONFIG_PATH
#define DEFAULT_CONFIG_PATH "webserv.conf"

#undef DEFAULT_MAX_BODY_SIZE
#define DEFAULT_MAX_BODY_SIZE 1048576 /* 1MiB */
#undef DEFAULT_SERVER_ROOT
#define DEFAULT_SERVER_ROOT "/www/"
#undef DEFAULT_SERVER_PORT
#define DEFAULT_SERVER_PORT 80
#undef DEFAULT_SERVER_HOST
#define DEFAULT_SERVER_HOST "0.0.0.0"
#undef DEFAULT_UPLOAD_PATH
#define DEFAULT_UPLOAD_PATH "/uploads/"

/* Forward declarations ***************************************************** */

/* Enumerations ************************************************************* */

/* Type definitions ********************************************************* */

/* Interfaces *************************************************************** */

#include "base/base.ipp"
#include "base/io.ipp"
#include "base/io.hpp"

// #include "base/behavioral/"
// #include "base/behavioral/"
// #include "base/behavioral/"
// #include "base/behavioral/"
// #include "base/behavioral/"
// #include "base/behavioral/"
// #include "base/behavioral/"
// #include "base/behavioral/"
#include "base/behavioral/Observer.ipp"
#include "base/behavioral/Visitor.ipp"

#include "base/creational/Builder.ipp"
#include "base/creational/Factory.ipp"
#include "base/creational/Prototype.ipp"

/* Base classes ************************************************************* */

/* Class includes *********************************************************** */

#endif
