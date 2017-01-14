NAME = environ
CC = gcc
CFLAGS = -c

all: compile

compile: environ.o option.o
	$(CC) environ.o option.o -o $(NAME)

environ.o: environ.c
	$(CC) $(CFLAGS) environ.c

option.o: option.c
	$(CC) $(CFLAGS) option.c

test:
	./$(NAME)
	./$(NAME) -h
	./$(NAME) --set test=test
	./$(NAME) -i HOME
	./$(NAME) --clear
	./$(NAME) --del asdasda
	./$(NAME) -a test -v test
	./$(NAME) -a test
	./$(NAME) -v test

clean:
	rm -rf *o $(NAME)