task: task7

all: task1 task2 task3 task4 task5 task6 task7

task1: task1.c
	gcc task1.c -o task1.out

task2: task2.c
	gcc task2.c -o task2.out

task3: task3.c
	gcc task3.c -o task3.out

task4: task4.c
	gcc task4.c -o task4.out -D DEBUG

task5: task5.c
	gcc task5.c -o task5.out

task6: task6.c
	gcc task6.c -o task6.out

task7: task7.c task7_c.c
	gcc task7.c -o task7.out -lm
	gcc task7_c.c -o task7_c.out -lm

clean:
	rm -rf *out
