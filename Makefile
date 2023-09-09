NAME := ircserv

CXX := c++

CXXFLAGS := -Wall -Werror -Wextra -std=c++98

SRCDIR := source

OBJDIR := objs

HEADERS := $(SRCDIR)/includes

SRCS = $(shell find $(SRCDIR) -name '*.cpp')

OBJ := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

DIRS := $(sort $(dir $(OBJ)))

all: $(OBJDIR) $(NAME)

$(OBJDIR): $(DIRS)
	mkdir -p $(DIRS)

$(DIRS):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADERS) -c $< -o $@

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: CXXFLAGS += -g -fno-limit-debug-info
debug: CXX = clang++
debug: re

.PHONY: all, clean, fclean, re, debug
