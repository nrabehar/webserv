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

DIRDATA = ./
DATASRC = data.cpp
SRC += $(addprefix $(DIRDATA), $(DATASRC))

DIRCONFIG = config/
DIRLOCATION = location/
DIRNETWORK = network/
DIRPARSER = parser/
DIRSERVER = server/
DIRVALIDATOR = validator/

# CONFIGSRC += SrvConfig.cpp
# CONFIGSRC += LctConfig.cpp
# CONFIGSRC += Cfg.cpp
# LOCATIONSRC += Location.cpp
NETWORKSRC += Ip.cpp
PARSERSRC += IpParse.cpp
# SERVERSRC += Server.cpp
# VALIDATORSRC += CfgValid.cpp
VALIDATORSRC += IpValid.cpp

SRC += $(addprefix $(DIRCONFIG), $(CONFIGSRC))
SRC += $(addprefix $(DIRLOCATION), $(LOCATIONSRC))
SRC += $(addprefix $(DIRNETWORK), $(NETWORKSRC))
SRC += $(addprefix $(DIRPARSER), $(PARSERSRC))
SRC += $(addprefix $(DIRSERVER), $(SERVERSRC))
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
