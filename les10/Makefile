CC = gcc
CFLAGS = -D DEBUG
NAME = task

cur: task4

all: task1 task2 task3 task4

task1: task1_1.c task1_2.c
	$(CC) task1_1.c -o $(NAME)1_1.out $(CFLAGS)
	$(CC) task1_2.c -o $(NAME)1_2.out $(CFLAGS)

task2: task2.c 
	$(CC) task2.c -o $(NAME)2.out $(CFLAGS)

task3: task3.c 
	$(CC) task3.c -o $(NAME)3.out $(CFLAGS)

task4: task4.c 
	$(CC) task4.c -o $(NAME)4.out $(CFLAGS)

clean:
	rm -rf *.out