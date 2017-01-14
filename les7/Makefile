CFLAGS = -g -Wall -lpthread -lm -D DEBUG
CC = gcc

task: task6

all: task1 task2 task3 task4 task5 task6

task1: task1.c
	$(CC) task1.c -o task1.out $(CFLAGS)

task2: task2.c
	$(CC) task2.c -o task2.out $(CFLAGS)

task3: task3.c
	$(CC) task3.c -o task3.out $(CFLAGS)

task4: task4.c
	$(CC) task4.c -o task4.out $(CFLAGS)

task5: task5.c
	$(CC) task5.c -o task5.out $(CFLAGS)

task6: task6.c
	$(CC) task6.c -o task6.out $(CFLAGS)

clean:
	rm -rf *.out