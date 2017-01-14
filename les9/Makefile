CFLAGS = -g -Wall -lpthread -D DEBUG
CC = gcc

all: task1 task2

task1: task1.c
	$(CC) task1.c -o task1.out $(CFLAGS) 

task2: task2.c
	$(CC) task2.c -o task2.out $(CFLAGS)

clean:
	rm -rf *.out