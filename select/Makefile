NAME		:= select
OBJ_DIR		:= obj

SRC_DIR		:= ./srcs
INC_DIR		:= ./includes

CC			:= cc
CCFLAGS	:= -Wall -Werror -Wextra
CCFLAGS	+= -g -fsanitize=address

SRCS_NAME	:= mini_serv.c

SRCS		:= $(addprefix $(SRC_DIR)/, $(SRCS_NAME))
OBJECTS		:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME):	$(OBJECTS)
	$(CC) $(CCFLAGS) -I $(INC_DIR) -o $(NAME) $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all fclean clean re
