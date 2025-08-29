# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sel <sel@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/29 14:49:52 by sel               #+#    #+#              #
#    Updated: 2025/08/29 14:52:18 by sel              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libarena.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I include

SRC = src/collector_init.c \
      src/collector_alloc.c \
      src/collector_realloc.c \
      src/collector_finalizer.c \
      src/collector_free.c \
      src/collector_scope.c

OBJ = $(SRC:.c=.o)

# Default target: compile static library
all: $(NAME)

# Create static library from object files
$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

# Compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove object files
clean:
	rm -f $(OBJ)

# Remove object files and static library
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all