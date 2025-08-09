NAME = webserv

SRC_DIR = src
OBJ_DIR = obj

CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

S_CONF = \
	$(SRC_DIR)/conf/Config.cpp \
	$(SRC_DIR)/conf/Location.cpp \
	$(SRC_DIR)/conf/Server.cpp \
	$(SRC_DIR)/conf/ConfigParser.cpp \

S_TOOL = \
	$(SRC_DIR)/tools/String.cpp \
	$(SRC_DIR)/tools/Checker.cpp \
	$(SRC_DIR)/tools/WException.cpp \

S_NET = \
	$(SRC_DIR)/network/Socket.cpp \
	$(SRC_DIR)/network/NetworkManager.cpp \
	$(SRC_DIR)/network/ConnectionManager.cpp \

S_EVENT = \
	$(SRC_DIR)/event/Signal.cpp \
	$(SRC_DIR)/event/EventManager.cpp \

S_CLIENT = \
	$(SRC_DIR)/client/Client.cpp \
	$(SRC_DIR)/client/ClientManager.cpp \

S_HTTP = \
	$(SRC_DIR)/http/HttpServer.cpp \
	$(SRC_DIR)/http/HttpRequest.cpp \
	$(SRC_DIR)/http/RequestManager.cpp \


SRCS = \
	$(SRC_DIR)/Webserv.cpp \
	$(S_NET) \
	$(S_CONF) \
	$(S_TOOL) \
	$(S_EXCEPTIONS) \
	$(S_EVENT) \
	$(S_CLIENT) \
	$(S_HTTP) \

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

leak: $(NAME) clean
	@clear
	@valgrind --show-leak-kinds=all --leak-check=full --track-origins=yes --track-fds=yes ./$(NAME) conf/webserv.conf


.PHONY: all clean fclean re leak