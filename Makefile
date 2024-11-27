NAME		=	webserv

CPP			=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98
RM			=	rm -rf

SRCS		=	Webserv.cpp main.cpp
OBJS		=	$(SRCS:.cpp=.o)

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CPP) $(CPPFLAGS) $(OBJS) -o $@

%.o:			%.cpp
				$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
