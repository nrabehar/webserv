NAME = webserv

DIRSRC = src

DIROBJ = obj

CONFIGFILE = webserv.conf

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror
STD98 = -std=c++98
CXXFLAGS += $(STD98)

MAIN_SRC = $(DIRSRC)/main.cpp

LOGFILE = webserv.log

CONF_DIR = conf
CONF_SRC = \
	Config.cpp \
	ConfigFile.cpp \

CONF_SRCS = $(addprefix $(CONF_DIR)/, $(CONF_SRC))

CORE_DIR = core
CORE_SRC = \
	Location.cpp \
	Webserv.cpp \

CORE_SRCS = $(addprefix $(CORE_DIR)/, $(CORE_SRC))

TOOLS_DIR = tools
TOOLS_SRC = \
	Logger.cpp \
	Utils.cpp \

TOOLS_SRCS = $(addprefix $(TOOLS_DIR)/, $(TOOLS_SRC))

SUBDIRS = \
	$(CONF_DIR) \
	$(CORE_DIR) \
	$(TOOLS_DIR) \

INCLUDES = $(addprefix -I./$(DIRSRC)/, $(SUBDIRS))
CXXFLAGS += $(INCLUDES)

SRC = \
	$(CONF_SRCS) \
	$(CORE_SRCS) \
	$(TOOLS_SRCS) \

SRCS = $(addprefix $(DIRSRC)/, $(SRC))
OBJ = $(SRC:%.cpp=%.o)
OBJS = $(addprefix $(DIROBJ)/, $(OBJ))

$(NAME): $(MAIN_SRC) $(OBJS)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(OBJS) -o $(NAME)

$(DIROBJ)/%.o: $(DIRSRC)/%.cpp
	@mkdir -pv $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

clean:
	@rm -rfv $(DIROBJ)

fclean:
	@rm -rfv $(DIROBJ) $(LOGFILE) $(NAME)

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
