# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sel <sel@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/29 14:49:46 by sel               #+#    #+#              #
#    Updated: 2025/08/29 14:57:09 by sel              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = test_libarena
CC = cc
CFLAGS = -Wall -Wextra -Werror -I ../include
LIB = ../libarena.a
SRC = main.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LIB) -fsanitize=address -g3 -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
