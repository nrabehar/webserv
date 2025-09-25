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
#include <list>
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

template <typename GuestType, typename HostType> class IGuest;
template <typename HostType, typename GuestType> class IHost;
template <typename Element> class IMemento;
template <typename Element> class IOriginator;
template <typename Element> class ICaretaker;
class ISignal;

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

#include "base/behavioral.ipp"


#include "tools/Logger.ipp"
#include "tools/Logger.hpp"
#include "tools/Signal.ipp"
#include "tools/Signal.hpp"
#include "tools/String.hpp"
#include "tools/Token.hpp"
#include "tools/TokenExtractor.hpp"

#include "tools/Lexer.hpp"


#include "core/EventLoop.hpp"
#include "core/File.ipp"
#include "core/Cache.ipp"
#include "core/Manager.ipp"

#include "core/node/Node.hpp"
#include "core/node/NodeValidator.hpp"

#include "core/directive/Directive.hpp"
#include "core/directive/DirectiveParser.hpp"
#include "core/directive/DirectiveRegistry.hpp"
#include "core/directive/DirectiveChecker.hpp"

#include "net/Server.hpp"


/* Class includes *********************************************************** */

#include "base.hpp"
#include "file/FileCache.hpp"
#include "file/FileHandler.hpp"

#include "config/Config.hpp"
#include "config/Server.hpp"
#include "config/Sanitizer.hpp"
#include "config/Parser.hpp"
#include "config/Merger.hpp"
#include "config/Validator.hpp"
#include "config/Transformer.hpp"


#endif
