NAME      := RPN
CPP       := c++
# CPP		  := g++
CPPFLAGS  := -Wall -Wextra -Werror -std=c++98
# CPPFLAGS  := -g

SRC       := main.cpp RPN.cpp
OBJ_DIR   := obj
OBJ       := $(addprefix $(OBJ_DIR)/,$(SRC:.cpp=.o))

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CPP) $(CPPFLAGS) $^ -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re