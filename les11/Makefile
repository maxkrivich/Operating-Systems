CC = gcc
CFLAGS = -D DEBUG
NAME = task

cur: task1

all: task1 task2

task1: task1.c 
	$(CC) task1.c -o $(NAME)1.out $(CFLAGS)
	$(CC) main.c -o main $(CFLAGS)

clean:
	rm -rf *.out