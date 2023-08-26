NAME := ircserv

CXX := c++

CXXFLAGS := -Wall -Werror -Wextra -std=c++98

SRCDIR = source
OBJDIR = objs

SRCS = $(shell find $(SRCDIR) -name '*.cpp')
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

DIRS = $(sort $(dir $(OBJ)))

LIBS := 

all: $(OBJDIR) $(NAME)

$(OBJDIR): $(DIRS)
	mkdir -p $(DIRS)

$(DIRS):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< -o $@

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: CXXFLAGS += -g
debug: re

.PHONY: all, clean, fclean, re, debug