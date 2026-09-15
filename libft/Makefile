# Output library name
NAME 		=	libft.a

# Directories
SRC_DIR 	=	src
OBJ_DIR 	=	obj
HF_DIR 		=	hf

# Source files
SRC_FILES 	=	ft_isalnum.c		ft_isascii.c	ft_isprint.c \
				ft_strlcat.c		ft_strlen.c		ft_toupper.c \
				ft_atoi.c			ft_isalpha.c	ft_isdigit.c \
				ft_tolower.c		ft_memset.c		ft_bzero.c \
				ft_memcpy.c			ft_memmove.c	ft_strchr.c \
				ft_strrchr.c		ft_strncmp.c	ft_memchr.c \
				ft_memcmp.c			ft_strnstr.c	ft_strlcat.c \
				ft_strlcpy.c		ft_strdup.c		ft_calloc.c \
				ft_substr.c			ft_strjoin.c	ft_strtrim.c \
				ft_putchar_fd.c		ft_putstr_fd.c	ft_putendl_fd.c \
				ft_putnbr_fd.c		ft_itoa.c		ft_strmapi.c \
				ft_lstsize.c		ft_lstnew.c		ft_lstadd_front.c\
				ft_lstadd_back.c	ft_lstiter.c	ft_lstlast.c\
				ft_lstdelone.c		ft_lstclear.c	ft_lstmap.c\
				get_next_line_bonus.c	ft_putf.c	ft_conversions.c \
				ft_striteri.c ft_split.c			ft_printf.c \
				get_next_line_utils_bonus.c			ft_strcmp.c

# Object files
OBJ_FILES	=	$(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Compiler and flags
CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror

# Default target
all	:				$(NAME)
				@echo > /dev/null

# Rule for creating the library
$(NAME)	:			$(OBJ_FILES)
				@ar rcs $@ $^
				@echo "Library $@ created successfully!"

# Rule for creating object files
$(OBJ_DIR)/%.o	:	$(SRC_DIR)/%.c | $(OBJ_DIR)
				@$(CC) $(CFLAGS) -I . -c $< -o $@

# Rule for creating the obj directory
$(OBJ_DIR)	:
				@mkdir -p $@

# Clean rule
clean	:
				@rm -rf $(OBJ_DIR)

# Full clean rule
fclean	:			clean
				@rm -f $(NAME)

# Rebuild rule
re	:				fclean all

# .PHONY rule to prevent conflicts with file names
.PHONY	:			all clean fclean re
