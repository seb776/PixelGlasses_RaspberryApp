NAME	=	RaspApp

SRC	=	main.c \

OBJ	=	$(SRC:.c=.o)

CC	=	g++

CFLAGS	=	-Wall -Wextra -I./ -fpermissive

LDFLAGS	=	-lX11 -lpng

all:	$(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -vf $(OBJ)

fclean: clean
	rm -vf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
