# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alexandervalencia <alexandervalencia@st    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 10:22:05 by theodeville       #+#    #+#              #
#    Updated: 2023/03/16 08:56:15 by alexanderva      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include srcs/server/config/server.mk
include srcs/client/config/Client.mk


NAME	= ircserv
HEADER	+= 

SRCS	+=	main.cpp

CC		=	c++
CFLAGS	=	-std=c++98 #-Wall -Wextra -Werror

OBJS	=	$(SRCS:.cpp=.o)

%.o:		%.cpp $(HEADER) Makefile
	$(CC)  $(CFLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS) $(HEADER)
	$(CC)  $(CFLAGS) $(HEADER) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re