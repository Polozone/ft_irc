# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: theodeville <theodeville@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 10:22:05 by theodeville       #+#    #+#              #
#    Updated: 2023/03/10 10:27:07 by theodeville      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include srcs/server/config/server.mk
include srcs/client/config/client.mk


NAME	= ircserv
HEADER	+= 

SRCS	+=	main.cpp

CC		=	c++
CFLAGS	=	-std=c++98 #-Wall -Wextra -Werror

OBJS	=	$(SRCS:.cpp=.o)

%.o:		%.cpp $(HEADERS) Makefile
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