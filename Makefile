NAME = webserv

DIRSRC = src
DIROBJ = obj

CONFIG_FILE = webserv.conf

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror
STD98 = -std=c++98
CXXFLAGS += $(STD98)

CONF_DIR = conf
TOOLS_DIR = tools
NETWORK_DIR = network
EVENT_DIR = event
CLIENT_DIR = client
HTTP_DIR = http
SERVER_DIR = server

MAIN_SRC = $(DIRSRC)/main.cpp

SRC_CONF = \
	Config.cpp \
	Location.cpp \
	ConfigParser.cpp \
	Server.cpp \

SRC_TOOLS = \
	String.cpp \
	Checker.cpp \
	WException.cpp \

SRC_NETWORK = \
	Socket.cpp \
	NetworkManager.cpp \
	ConnectionManager.cpp \

SRC_EVENT = \
	Signal.cpp \
	EventManager.cpp \

SRC_CLIENT = \
	Client.cpp \
	ClientManager.cpp \

SRC_HTTP = \
	HttpServer.cpp \
	HttpRequest.cpp \
	RequestManager.cpp \

SRC_SERVER = \
	Webserv.cpp \

SRCS_CONF = $(addprefix $(CONF_DIR)/, $(SRC_CONF))
SRCS_TOOLS = $(addprefix $(TOOLS_DIR)/, $(SRC_TOOLS))
SRCS_NETWORK = $(addprefix $(NETWORK_DIR)/, $(SRC_NETWORK))
SRCS_EVENT = $(addprefix $(EVENT_DIR)/, $(SRC_EVENT))
SRCS_CLIENT = $(addprefix $(CLIENT_DIR)/, $(SRC_CLIENT))
SRCS_HTTP = $(addprefix $(HTTP_DIR)/, $(SRC_HTTP))
SRCS_SERVER = $(addprefix $(SERVER_DIR)/, $(SRC_SERVER))

SRC = \
	$(SRCS_NETWORK) \
	$(SRCS_CONF) \
	$(SRCS_TOOLS) \
	$(SRCS_EVENT) \
	$(SRCS_CLIENT) \
	$(SRCS_HTTP) \
	$(SRCS_SERVER)

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
	@rm -rfv $(DIROBJ)
	@rm -fv $(NAME)

re:
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

run: $(NAME)
	@./$(NAME) $(CONFIG_FILE)

leak: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes ./$(NAME) $(CONFIG_FILE)

.PHONY: all clean fclean re run leak
