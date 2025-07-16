# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/16 10:20:26 by mmitkovi          #+#    #+#              #
#    Updated: 2025/07/16 10:20:29 by mmitkovi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = cc
CFLAGS = -Wall -Wextra -Werror -g

UTILS = ./utils/checkers.c ./utils/helpers.c ./utils/routines.c ./utils/sim_utils.c
SRC = ./src/init_philos.c ./src/simulation.c

UTILOBJS = $(patsubst %.c, ./objs/%.o, $(notdir $(UTILS)))
SRCOBJS = $(patsubst %.c, ./objs/%.o, $(notdir $(SRC)))
OBJS = $(UTILOBJS) $(SRCOBJS)

LIBR = philo.a

NAME = philo

./objs/%.o: ./utils/%.c
	@mkdir -p objs
	@$(CC) $(CFLAGS) -c $< -o $@

./objs/%.o: ./src/%.c
	@mkdir -p objs
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBR)
	@$(CC) $(CFLAGS) main.c objs/$(LIBR) -o $(NAME) -lc
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