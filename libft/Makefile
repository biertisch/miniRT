# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 12:50:42 by bliu              #+#    #+#              #
#    Updated: 2025/10/03 21:36:01 by bliu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRCS = ft_debug.c ft_atoi.c ft_bzero.c ft_calloc.c ft_exit.c ft_free.c ft_get_next_line.c ft_isalnum.c  		\
	   ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_isspace.c ft_itoa.c  	\
	   ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c 	\
	   ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c 	\
	   ft_split.c ft_split_cmd.c ft_split_cmd_keep.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c 	\
	   ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c 	ft_strcmp.c \
	   ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c  ft_arraylen.c\
	   ft_count_words.c ft_toupper.c ft_printf_utils.c ft_printf.c ft_strtoll.c
	   
BSRCS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c	\
	   ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

OBJS = $(SRCS:.c=.o)

BOBJS = $(BSRCS:.c=.o)

HEADERS = libft.h
DEBUG ?= 0
CFLAGS = -g -Wall -Wextra -Werror -MMD

ifeq ($(DEBUG),1)
	CFLAGS += -DDEBUG=$(DEBUG)
else
	CFLAGS += -DDEBUG=$(DEBUG)
endif

all: $(NAME)

$(NAME): $(OBJS)
	ar -rcs $(NAME) $(OBJS)

bonus : .bonus

.bonus: $(OBJS) $(BOBJS)
	ar -rcs $(NAME) $(OBJS) $(BOBJS)
	@touch .bonus

%.o: %.c $(HEADERS)
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BOBJS)
	rm -f $(OBJS:.o=.d) $(BOBJS:.o=.d)
	@rm -f .bonus

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(OBJS:.o=.d)
-include $(BOBJS:.o=.d)

.PHONY: all clean fclean re