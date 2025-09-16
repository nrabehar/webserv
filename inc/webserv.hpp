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

#ifndef DEFAULT_SERVER_CONFIG
# define DEFAULT_SERVER_CONFIG 1
# define DEFAULT_MAX_BODY_SIZE 1048576 /* 1MB */
# define DEFAULT_SERVER_ROOT "/www/"
# define DEFAULT_SERVER_PORT 80
# define DEFAULT_SERVER_HOST "0.0.0.0"
# define DEFAULT_UPLOAD_PATH "/uploads/"
#endif

/* Forward declarations ***************************************************** */

/* Enumerations ************************************************************* */

/* Type definitions ********************************************************* */

/* Interfaces *************************************************************** */

#include "data.ipp"
#include "data.hpp"

#include "network/Ip.ipp"
#include "config/Cfg.ipp"
#include "parser/IpParse.ipp"
// #include "validator/CfgValid.ipp"
#include "validator/IpValid.ipp"

/* Base classes ************************************************************* */

#include "network/Ip.hpp"
#include "config/Cfg.hpp"
#include "parser/IpParse.hpp"
// #include "validator/CfgValid.hpp"
#include "validator/IpValid.hpp"

/* Class includes *********************************************************** */

#endif
