NAME = webserv
DIRSRC = src/
DIROBJ = obj/
DIRINC = inc/
CONFIGFILE = webserv.conf
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror
STD98 = -std=c++98
CPPFLAGS += $(STD98)
SRCMAIN = $(DIRSRC)webserv.cpp
LOGFILE = webserv.log

DIRAGENT = agent/
DIRBASE = base/
DIRFILE = file/
DIRCACHE = cache/
DIRBUILDER = builder/
DIRCLIENT = client/
DIRCONFIG = config/
DIRCORE = core/
DIRHANDLER = handler/
DIRHTTP = http/
DIRINTERFACE = interface/
DIRLOCATION = location/
DIRNETWORK = network/
DIRPARSER = parser/
DIRSERVER = server/
DIRTOOLS = tools/
DIRVALIDATOR = validator/

BASESRC += base.cpp
TOOLSSRC += Logger.cpp
TOOLSSRC += Signal.cpp
TOOLSSRC += Token.cpp
TOOLSSRC += TokenExtractor.cpp
TOOLSSRC += Lexer.cpp

CORESRC += directive/Directive.cpp
CORESRC += directive/DirectiveParser.cpp
CORESRC += directive/DirectiveRegistry.cpp

CORESRC += node/NodeChecker.cpp

BASE_FILESRC += File.cpp \

BASE_CACHESRC += Cache.cpp \

FILESRC += FileCache.cpp \
				FileHandler.cpp \

CONFIGSRC += Config.cpp \
						ConfigMerger.cpp \
						ConfigParser.cpp \
						ConfigValidator.cpp

BASESRC += base.cpp

SRC += $(addprefix $(DIRAGENT), $(AGENTSRC))

SRC += $(addprefix $(DIRBASE), $(BASESRC))
SRC += $(addprefix $(DIRBASE), $(addprefix $(DIRINTERFACE), $(BASE_INTERFACESRC)))

SRC += $(addprefix $(DIRFILE), $(FILESRC))

SRC += $(addprefix $(DIRBASE), $(addprefix $(DIRFILE), $(BASE_FILESRC)))

SRC += $(addprefix $(DIRBASE), $(addprefix $(DIRCACHE), $(BASE_CACHESRC)))

SRC += $(addprefix $(DIRBUILDER), $(BUILDERSRC))

SRC += $(addprefix $(DIRCLIENT), $(CLIENTSRC))

SRC += $(addprefix $(DIRCONFIG), $(CONFIGSRC))

SRC += $(addprefix $(DIRCORE), $(CORESRC))

SRC += $(addprefix $(DIRHANDLER), $(HANDLERSRC))

SRC += $(addprefix $(DIRHTTP), $(HTTPSRC))

SRC += $(addprefix $(DIRINTERFACE), $(INTERFACESRC))

SRC += $(addprefix $(DIRLOCATION), $(LOCATIONSRC))

SRC += $(addprefix $(DIRNETWORK), $(NETWORKSRC))

SRC += $(addprefix $(DIRPARSER), $(PARSERSRC))

SRC += $(addprefix $(DIRSERVER), $(SERVERSRC))

SRC += $(addprefix $(DIRTOOLS), $(TOOLSSRC))

SRC += $(addprefix $(DIRVALIDATOR), $(VALIDATORSRC))

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
