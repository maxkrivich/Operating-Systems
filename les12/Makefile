CC = gcc
CFLAG = -g -Wall

all: task1 task2

task1: task1.c
	$(CC) task1.c $(CFLAG) -o task1.out

task2: task2_1.c task2_2.c
	$(CC) task2_1.c $(CFLAG) -o task2_1.out
	$(CC) task2_2.c $(CFLAG) -o task2_2.out

clean:
	rm -rf *.out