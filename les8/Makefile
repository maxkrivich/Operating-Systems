CFLAGS = -g -Wall -lpthread -lm -D DEBUG
CC = gcc

all: task1 task2 task3

task1: task1.c
	$(CC) task1.c -o task1.out $(CFLAGS) 

task2: task2.c
	$(CC) task2.c -o task2.out $(CFLAGS) -lm

task3: task3.c
	$(CC) task3.c -o task3.out $(CFLAGS)

clean:
	rm -rf *.out
