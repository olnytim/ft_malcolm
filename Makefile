# **************************************************************************** #
#                                                                            #
#   Makefile for ft_malcolm (ARP spoofing / MITM introduction).              #
#                                                                            #
# **************************************************************************** #

NAME		=	ft_malcolm

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g

SRC_DIR		=	src
OBJ_DIR		=	obj

SRC			=	main.c \
				args.c \
				net.c \
				arp.c \
				utils.c

OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

HEADER		=	$(SRC_DIR)/ft_malcolm.h

LIBFT_DIR	=	libft
LIBFT		=	$(LIBFT_DIR)/libft.a

all:			$(NAME)
				@echo > /dev/null

$(LIBFT):
				@$(MAKE) -C $(LIBFT_DIR)

$(NAME):		$(LIBFT) $(OBJ)
				@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
				@echo "\033[36m$(NAME) was created successfully!\033[0m"

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(CC) $(CFLAGS) -c $< -o $@

bonus:			all

clean:
				@rm -rf $(OBJ_DIR)
				@$(MAKE) -C $(LIBFT_DIR) clean

fclean:			clean
				@rm -f $(NAME)
				@$(MAKE) -C $(LIBFT_DIR) fclean

re:				fclean all

.PHONY:			all bonus clean fclean re
