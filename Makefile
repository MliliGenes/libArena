# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sel <sel@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/29 14:49:52 by sel               #+#    #+#              #
#    Updated: 2025/08/29 15:31:12 by sel              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libarena.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I include

# Source files matching your actual file structure
SRC = src/arena_alloc.c \
      src/arena_finalizer.c \
      src/arena_free.c \
      src/arena_init.c \
      src/arena_realloc.c \
      src/arena_scope.c

OBJ = $(SRC:.c=.o)

# ANSI color codes for better output
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m # No Color

# Default target: compile static library
all: $(NAME)

# Create static library from object files
$(NAME): $(OBJ)
	@echo "$(YELLOW)Creating static library $(NAME)...$(NC)"
	ar rcs $(NAME) $(OBJ)
	@echo "$(GREEN)✓ $(NAME) created successfully!$(NC)"

# Compile .c files to .o
%.o: %.c
	@echo "$(YELLOW)Compiling $<...$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@

# Remove object files
clean:
	@echo "$(YELLOW)Cleaning object files...$(NC)"
	rm -f $(OBJ)
	@echo "$(GREEN)✓ Object files cleaned!$(NC)"

# Remove object files and static library
fclean: clean
	@echo "$(YELLOW)Removing $(NAME)...$(NC)"
	rm -f $(NAME)
	@echo "$(GREEN)✓ Full clean completed!$(NC)"

# Rebuild everything
re: fclean all

# Debug target with debugging flags
debug: CFLAGS += -g -fsanitize=address
debug: re

# Test target to build and run tests
test: $(NAME)
	@echo "$(YELLOW)Building and running tests...$(NC)"
	$(MAKE) -C tests
	@echo "$(GREEN)✓ Tests completed!$(NC)"

# Install header to system (optional)
install: $(NAME)
	@echo "$(YELLOW)Installing libarena...$(NC)"
	cp $(NAME) /usr/local/lib/
	cp include/lib_arena.h /usr/local/include/
	@echo "$(GREEN)✓ LibArena installed to /usr/local/$(NC)"

# Uninstall from system
uninstall:
	@echo "$(YELLOW)Uninstalling libarena...$(NC)"
	rm -f /usr/local/lib/$(NAME)
	rm -f /usr/local/include/lib_arena.h
	@echo "$(GREEN)✓ LibArena uninstalled!$(NC)"

# Show help
help:
	@echo "$(GREEN)LibArena Build System$(NC)"
	@echo "Available targets:"
	@echo "  $(YELLOW)all$(NC)       - Build the static library (default)"
	@echo "  $(YELLOW)clean$(NC)     - Remove object files"
	@echo "  $(YELLOW)fclean$(NC)    - Remove object files and library"
	@echo "  $(YELLOW)re$(NC)        - Rebuild everything"
	@echo "  $(YELLOW)debug$(NC)     - Build with debug flags"
	@echo "  $(YELLOW)test$(NC)      - Build library and run tests"
	@echo "  $(YELLOW)install$(NC)   - Install to /usr/local"
	@echo "  $(YELLOW)uninstall$(NC) - Remove from /usr/local"
	@echo "  $(YELLOW)help$(NC)      - Show this help message"

# Declare phony targets
.PHONY: all clean fclean re debug test install uninstall help