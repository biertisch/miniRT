# Makefile for miniRT validator

NAME = validator
CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ../../libft
LIBFT_A = $(LIBFT_DIR)/libft.a

SRCS = parser_main.c validator.c schema.c parse_utils.c parse_line.c parser.c
OBJS = $(SRCS:.c=.o)

INC = -I../../includes

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -I../../libft -c $< -o $@

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) DEBUG=$(D)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
