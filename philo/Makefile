# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/16 10:20:26 by mmitkovi          #+#    #+#              #
#    Updated: 2025/07/17 10:33:35 by mmitkovi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Executable name
NAME = philo

# Compiler and flags
CC = cc
# The -pthread flag is required for compiling and linking with the POSIX threads library.
CFLAGS = -Wall -Wextra -Werror -pthread

# Add -g flag for debug builds
ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

# Source files
SRCS = philo.c \
	   init.c \
	   utils.c \
	   helpers.c

# Object files directory
OBJS_DIR = objs

# Generate object file names from source files, placing them in the object directory
# e.g., 'philo.c' becomes 'objs/philo.o'
OBJS = $(patsubst %.c, $(OBJS_DIR)/%.o, $(SRCS))

# Default rule, executed when you just run `make`
all: $(NAME)

# Rule to link the final executable from all object files
$(NAME): $(OBJS)
	@echo "Linking..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(NAME) created successfully!"

# Rule to compile .c source files into .o object files.
# This also depends on philo.h, so changing the header recompiles the sources.
$(OBJS_DIR)/%.o: %.c philo.h
	@mkdir -p $(OBJS_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule to remove object files
clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJS_DIR)

# Rule to remove object files and the final executable
fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME)

# Rule to rebuild the project from scratch
re: fclean all

# A bonus rule to compile with debug flags (`make debug`)
debug:
	@$(MAKE) all DEBUG=1

# Declare rules that do not correspond to files
.PHONY: all clean fclean re debug