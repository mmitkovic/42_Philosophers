# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/16 10:20:26 by mmitkovi          #+#    #+#              #
#    Updated: 2025/07/17 09:56:30 by mmitkovi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = helpers.c \
		init.c \
		philo.c \
		utils.c \
#SRC = ./src/init_philos.c ./src/simulation.c

#UTILOBJS = $(patsubst %.c, ./objs/%.o, $(notdir $(UTILS)))
SRCS_OBJS = $(patsubst %.c, ./objs/%.o, $(notdir $(SRCS)))
OBJS = $(SRC_OBJS)

LIBR = philo.a

NAME = philo

./objs/%.o: %.c
	@mkdir -p objs
	@$(CC) $(CFLAGS) -c $< -o $@

#./objs/%.o: ./src/%.c
#	@mkdir -p objs
#	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBR)
	@$(CC) $(CFLAGS) philo.c objs/$(LIBR) -o $(NAME) -lc
	@echo "Build complete"

all: $(NAME)

$(LIBR): $(OBJS)
	@ar rcs objs/$(LIBR) $(OBJS)

clean:
	@rm -f $(OBJS)
	@echo "Clean complete"

fclean: clean
	@rm -f ./objs/$(LIBR)
	@rm -f $(NAME)
	@rm -d objs
	@echo "Full clean complete"

re: fclean $(NAME)

.PHONY: all clean fclean re