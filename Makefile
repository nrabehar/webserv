NAME = webserv

# Directories
SRC_DIR = src
OBJ_DIR = obj

CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

S_CONF = \
	$(SRC_DIR)/Config.cpp \
	$(SRC_DIR)/Location.cpp \
	$(SRC_DIR)/Server.cpp \

S_EXCEPTIONS = \
	$(SRC_DIR)/WException.cpp \


S_TOOL = \
	$(SRC_DIR)/ConfigParser.cpp \
	$(SRC_DIR)/String.cpp \
	$(SRC_DIR)/Checker.cpp \

S_NET = \
	$(SRC_DIR)/Webserv.cpp \
	$(SRC_DIR)/Socket.cpp \

S_EVENT = \
	$(SRC_DIR)/Signal.cpp \
	$(SRC_DIR)/EventManager.cpp \

S_CLIENT = \
	$(SRC_DIR)/Client.cpp \
	$(SRC_DIR)/ClientManager.cpp \



SRCS = \
	$(S_NET) \
	$(S_CONF) \
	$(S_TOOL) \
	$(S_EXCEPTIONS) \
	$(S_EVENT) \
	$(S_CLIENT) \

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))


$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $^


all: $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME) clean
	@clear
	@./$(NAME) conf/webserv.conf

.PHONY: all clean fclean re