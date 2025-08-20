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

SRC = \

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
