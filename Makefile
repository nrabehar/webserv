NAME = webserv
DIRSRC = src/
DIROBJ = obj/
DIRINC = inc/
CONFIGFILE = conf/webserv.conf
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror
STD98 = -std=c++98
CPPFLAGS += $(STD98)
SRCMAIN = $(DIRSRC)webserv.cpp
LOGFILE = webserv.log

DIRCONFIG = config/
DIRCORE = core/
DIRFILE = file/
DIRHANDLER = handler/
DIRHTTP = http/
DIRNET = net/
DIRTOOLS = tools/

TOOLSSRC += Logger.cpp
TOOLSSRC += Signal.cpp
TOOLSSRC += String.cpp
TOOLSSRC += Time.cpp
TOOLSSRC += Token.cpp
TOOLSSRC += TokenExtractor.cpp
TOOLSSRC += Lexer.cpp
TOOLSSRC += NetUtil.cpp
TOOLSSRC += Mime.cpp
TOOLSSRC += UI.cpp

CORESRC += directive/Directive.cpp
CORESRC += directive/DirectiveParser.cpp
CORESRC += directive/DirectiveRegistry.cpp
CORESRC += memory/memory.cpp
CORESRC += EventLoop.cpp
CORESRC += EventHandler.cpp
CORESRC += Poller.cpp
CORESRC += Buffer.cpp
CORESRC += Cache.cpp
CORESRC += node/NodeChecker.cpp

FILESRC += File.cpp
FILESRC += FileCache.cpp
FILESRC += FileHandler.cpp

CONFIGSRC += Config.cpp
CONFIGSRC += Sanitizer.cpp
CONFIGSRC += Parser.cpp
CONFIGSRC += Merger.cpp
CONFIGSRC += Validator.cpp
CONFIGSRC += Server.cpp
CONFIGSRC += Transformer.cpp

NETSRC += Server.cpp
NETSRC += Client.cpp

HTTPSRC += Request.cpp
HTTPSRC += Response.cpp
HTTPSRC += Parser.cpp

HANDLERSRC += RequestHandler.cpp
HANDLERSRC += UriHandler.cpp
HANDLERSRC += ErrorHandler.cpp
HANDLERSRC += StaticHandler.cpp
HANDLERSRC += MethodHandler.cpp
HANDLERSRC += CgiHandler.cpp

SRC += $(addprefix $(DIRCONFIG), $(CONFIGSRC))

SRC += $(addprefix $(DIRCORE), $(CORESRC))

SRC += $(addprefix $(DIRFILE), $(FILESRC))

SRC += $(addprefix $(DIRHANDLER), $(HANDLERSRC))

SRC += $(addprefix $(DIRHTTP), $(HTTPSRC))

SRC += $(addprefix $(DIRNET), $(NETSRC))

SRC += $(addprefix $(DIRTOOLS), $(TOOLSSRC))

INCFILE = webserv.hpp
INC = -I./$(DIRINC) --include $(DIRINC)$(INCFILE)
CPPFLAGS += $(INC)

SRCS = $(addprefix $(DIRSRC), $(SRC))
OBJ = $(SRC:%.cpp=%.o)
OBJS = $(addprefix $(DIROBJ), $(OBJ))

$(NAME): $(SRCMAIN) $(OBJS)
	$(CPP) $(CPPFLAGS) $(SRCMAIN) $(OBJS) -o $(NAME)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	@mkdir -pv $(@D)
	$(CPP) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

clean:
	@rm -rfv $(DIROBJ)

fclean:
	@rm -fv $(LOGFILE)
	@rm -rfv $(DIROBJ)
	@rm -fv $(NAME)

re:
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

run: $(NAME)
	./$(NAME) $(CONFIGFILE)

VGF = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes

leak: $(NAME)
	valgrind $(VGF) ./$(NAME) $(CONFIGFILE)

.PHONY: all clean fclean re run leak
