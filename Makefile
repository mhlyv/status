CC = tcc
NAME = status
CFLAGS = -Wall

$(NAME): status.c
	$(CC) -o $(NAME) $(CFLAGS) $<
