NAME		:= non-blocking
OBJ_DIR		:= obj

SRC_DIR		:= ./srcs
INC_DIR		:= ./includes

CXX			:= c++
CXXFLAGS	:= -Wall -Werror -Wextra -std=c++98
CXXFLAGS	+= -g -fsanitize=address

SRCS_NAME	:= main.cpp

SRCS		:= $(addprefix $(SRC_DIR)/, $(SRCS_NAME))
OBJECTS		:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME):	$(OBJECTS)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -o $(NAME) $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all fclean clean re
