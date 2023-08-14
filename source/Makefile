NAME := ircserv

CXX := c++

CXXFLAGS := -Wall -Werror -Wextra -std=c++98

INC_PATH := includes

HEADERS := includes/Server.hpp \
			includes/common.hpp \
			includes/Client.hpp \

SRCS := ircserv.cpp \
		Server.cpp \
		Client.cpp \

OBJ := $(SRCS:%.cpp=%.o)

LIBS := 

all: $(NAME)

$(NAME): $(OBJ) $(HEADERS)
	$(CXX) $(OBJ) -I $(INC_PATH) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: CXXFLAGS += -g
debug: re

.PHONY: all, clean, fclean, re, debug
