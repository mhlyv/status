CC = tcc
NAME = status
CFLAGS = -Wall -Wextra -pedantic

$(NAME): status.c config.h
	$(CC) -o $(NAME) $(CFLAGS) $<
