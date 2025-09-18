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

#undef DEFAULT_CONFIG_FILE_NAME
#define DEFAULT_CONFIG_FILE_NAME "webserv.conf"

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

class IConfig;
class IConfigFile;
class IFile;
class IFileHandler;
template <typename GuestType, typename HostType> class IGuest;
template <typename HostType, typename GuestType> class IHost;
class ILogger;
template <typename Element> class IMemento;
template <typename Element> class IOriginator;
template <typename Element> class ICaretaker;
class ISignal;

class Config;
class ConfigFile;
class DefaultConfig;
class Logger;
class Signal;

/* Enumerations ************************************************************* */

enum e_LogLevel
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
};

/* Type definitions ********************************************************* */

/* Interfaces *************************************************************** */

#include "base.hpp"

#include "base/behavioral.ipp"

#include "core/File.ipp"

#include "config/Config.ipp"
#include "config/ConfigFile.ipp"
// #include "config/DefaultConfig.ipp"

#include "core/FileHandler.ipp"

#include "tools/Logger.ipp"
#include "tools/Signal.ipp"

/* Template classes ********************************************************* */

/* Base classes ************************************************************* */

#include "config/Config.hpp"
#include "config/ConfigFile.hpp"
#include "config/DefaultConfig.hpp"

// #include "core/File.hpp"
// #include "core/FileHandler.hpp"

#include "tools/Logger.hpp"
#include "tools/Signal.hpp"

/* Class includes *********************************************************** */

#endif
