# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/12 02:55:49 by alepinto          #+#    #+#              #
#    Updated: 2023/05/03 06:05:12 by alepinto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRC =	ft_isalpha.c		\
	ft_isdigit.c		\
	ft_isspace.c		\
	ft_isalnum.c		\
	ft_isascii.c		\
	ft_isprint.c		\
	ft_strlen.c		\
	ft_memset.c 		\
	ft_bzero.c		\
	ft_memcpy.c		\
	ft_memmove.c		\
	ft_strlcpy.c		\
	ft_strlcat.c		\
	ft_toupper.c		\
	ft_tolower.c		\
	ft_strchr.c		\
	ft_strrchr.c		\
	ft_strncmp.c		\
	ft_memchr.c		\
	ft_memcmp.c		\
	ft_strtok.c		\
	ft_strnstr.c		\
	ft_free_matrix.c	\
	ft_atoi.c		\
	ft_calloc.c		\
	ft_realloc.c	\
	ft_strdup.c		\
	ft_substr.c		\
	ft_strjoin.c		\
	ft_strtrim.c		\
	ft_split.c		\
	ft_itoa.c		\
	ft_strmapi.c		\
	ft_striteri.c		\
	ft_putchar_fd.c		\
	ft_putstr_fd.c		\
	ft_putendl_fd.c		\
	ft_putnbr_fd.c		\
	get_next_line.c 	\
	get_next_line_utils.c	\
	ft_lstnew.c		\
	ft_lstadd_front.c	\
	ft_lstsize.c		\
	ft_lstlast.c		\
	ft_lstadd_back.c	\
	ft_lstdelone.c		\
	ft_lstclear.c		\
	ft_lstiter.c		\
	ft_lstmap.c		\

OBJ = $(SRC:%.c=%.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
NAME =	libft.a	\

all: $(NAME)

$(NAME): $(OBJ)
	ar -rcs $(NAME) $(OBJ)

clean:
	rm -rf $(OBJ) || true

fclean: clean
	rm -rf $(NAME) || true

re: fclean all


.PHONY: all clean fclean re
