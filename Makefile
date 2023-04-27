# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 10:22:05 by theodeville       #+#    #+#              #
#    Updated: 2023/04/27 10:07:39 by alexanderva      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include srcs/config/server.mk
include srcs/config/client.mk
include srcs/config/channel.mk
include srcs/config/utils.mk
include srcs/config/commands.mk


NAME	= ircserv
HEADER	+= 

SRCS	+=	main.cpp
MK_FILES = $(shell find srcs/config/ -maxdepth 1 -iname *.mk)

CC		=	c++
CFLAGS	=	-std=c++98 -g -Wall -Wextra -Werror

OBJS	=	$(SRCS:.cpp=.o)

%.o:		%.cpp $(HEADER) Makefile $(MK_FILES)
	$(CC) $(CFLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re
