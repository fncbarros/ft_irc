NAME := ircserv

CXX := c++

CXXFLAGS := -Wall -Werror -Wextra -std=c++98

HEADERS := Server.hpp

SRCS := ircserv.cpp \
		Server.cpp \

OBJ := $(SRCS:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -I $(HEADERS) -o $(NAME)

clean:
	rm $(OBJ)

fclean:

.PHONY: all, clean, fclean
