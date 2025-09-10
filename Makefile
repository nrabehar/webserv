NAME = webserv
DIRSRC = src
DIROBJ = obj
DIRINC = inc
CONFIGFILE = webserv.conf
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror
STD98 = -std=c++98
CPPFLAGS += $(STD98)

SRCMAIN = $(DIRSRC)/main.cpp
LOGFILE = webserv.log

DIRCGI = cgi
DIRCLIENT = client
DIRCONF = conf
DIRHANDLER = handler
DIRHTTP = http
DIRNETWORK = network
DIRSERVER = server
DIRTOOLS = tools

CGISRC = \

CLIENTSRC = \

CONFSRC = \
	Config.cpp \
	ConfigFile.cpp \

HANDLERSRC = \

HTTPSRC = \

NETWORKSRC = \

SERVERSRC = \

TOOLSSRC = \
	AFile.cpp \

CGISRCS = $(addprefix $(DIRCGI)/, $(CGISRC))
CLIENTSRCS = $(addprefix $(DIRCLIENT)/, $(CLIENTSRC))
CONFSRCS = $(addprefix $(DIRCONF)/, $(CONFSRC))
HANDLERSRCS = $(addprefix $(DIRHANDLER)/, $(HANDLERSRC))
HTTPSRCS = $(addprefix $(DIRHTTP)/, $(HTTPSRC))
NETWORKSRCS = $(addprefix $(DIRNETWORK)/, $(NETWORKSRC))
SERVERSRCS = $(addprefix $(DIRSERVER)/, $(SERVERSRC))
TOOLSSRCS = $(addprefix $(DIRTOOLS)/, $(TOOLSSRC))

SUBDIRS = \
	$(DIRCGI) \
	$(DIRCLIENT) \
	$(DIRCONF) \
	$(DIRHANDLER) \
	$(DIRHTTP) \
	$(DIRNETWORK) \
	$(DIRSERVER) \
	$(DIRTOOLS) \

SRC = \
	$(CGISRCS) \
	$(CLIENTSRCS) \
	$(CONFSRCS) \
	$(HANDLERSRCS) \
	$(HTTPSRCS) \
	$(NETWORKSRCS) \
	$(SERVERSRCS) \
	$(TOOLSSRCS) \

INCFILE = webserv.hpp
INC = -I./$(DIRINC) --include $(DIRINC)/$(INCFILE)
CPPFLAGS += $(INC)

SRCS = $(addprefix $(DIRSRC)/, $(SRC))
OBJ = $(SRC:%.cpp=%.o)
OBJS = $(addprefix $(DIROBJ)/, $(OBJ))

$(NAME): $(SRCMAIN) $(OBJS)
	$(CPP) $(CPPFLAGS) $(SRCMAIN) $(OBJS) -o $(NAME)

$(DIROBJ)/%.o: $(DIRSRC)/%.cpp
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

VGF = --leak-check=full --show-leak-kinds=all --track-origins=yes \
	--track-fds=yes --trace-children=yes

leak: $(NAME)
	valgrind $(VGF) ./$(NAME) $(CONFIGFILE)

log: $(NAME)
	./$(NAME) $(CONFIGFILE) > $(LOGFILE) 2>&1

.PHONY: all clean fclean re run leak log
