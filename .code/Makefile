# === WEBSERV ================================================================ #

# -> Program names

NAME				=	webserv
NAMETEST		=	$(NAME).test

# -> Directories

SRCDIR			=	src/
OBJDIR			=	obj/
INCDIR			=	inc/

# -> Config and Log Files

CONFIGFILE	=	webserv.conf
LOGFILE			=	webserv.log

# -> Compiler and flags

CXX					=	c++
FLAGS				=	-Wall -Wextra -Werror
STD98				=	-std=c++98

# -> Include file

INCLUDE			=	webserv.hpp
INCS				=	-I$(INCDIR) --include $(INCDIR)$(INCLUDE)
CXXFLAGS		=	$(FLAGS) $(STD98) $(INCS)

# -> Source subdirectories

BASEDIR			=	base/
CONFIGDIR		=	config/
FILEDIR			=	file/
HTTPDIR			=	http/
SERVERDIR		=	server/
CLIENTDIR		=	client/
HANDLERDIR	=	handler/
PARSERDIR		=	parser/
TOOLSDIR		=	tools/

# -> Source files by directory

BASESRC			+=
CONFIGSRC		+=
FILESRC			+=
HTTPSRC			+=
SERVERSRC		+=
CLIENTSRC		+=
HANDLERSRC	+=
PARSERSRC		+=
TOOLSSRC		+=	Utils.cpp

# -> Combine source files

SRCFILES		=	\
							$(addprefix $(BASEDIR), $(BASESRC)) \
							$(addprefix $(CONFIGDIR), $(CONFIGSRC)) \
							$(addprefix $(FILEDIR), $(FILESRC)) \
							$(addprefix $(HTTPDIR), $(HTTPSRC)) \
							$(addprefix $(SERVERDIR), $(SERVERSRC)) \
							$(addprefix $(CLIENTDIR), $(CLIENTSRC)) \
							$(addprefix $(HANDLERDIR), $(HANDLERSRC)) \
							$(addprefix $(PARSERDIR), $(PARSERSRC)) \
							$(addprefix $(TOOLSDIR), $(TOOLSSRC))

# -> Main files

SRCMAIN			=	$(SRCDIR)webserv.cpp

# -> Object files

OBJFILES		=	$(SRCFILES:%.cpp=$(OBJDIR)%.o)

# === TARGETS ================================================================ #

# -> Default target

all: $(NAME)

# -> Main program

$(NAME): $(OBJFILES) $(SRCMAIN)
	$(CXX) $(CXXFLAGS) $(SRCMAIN) $(OBJFILES) -o $(NAME)

# -> Object files compilation

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	@mkdir -pv $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === UTILITY TARGETS ======================================================== #

# -> Clean object files

clean:
	@rm -rfv $(OBJDIR)

# -> Full clean (objects + executables + logs)

fclean: clean
	@rm -fv $(NAME) $(NAMETEST) $(LOGFILE)

# -> Rebuild rule

re: fclean all

# -> Run the program

run: $(NAME)
	./$(NAME) $(CONFIGFILE)

# -> Valgrind flags

VFLAGS	=	--leak-check=full --show-leak-kinds=all --track-origins=yes \
					--track-fds=yes --trace-children=yes

# -> Memory leak check for main program

leak: $(NAME)
	valgrind $(VFLAGS) ./$(NAME) $(CONFIGFILE)

# === PHONY ================================================================== #

.PHONY: all clean fclean re run leak
