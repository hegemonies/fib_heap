all:
	gcc -std=c99 -Wall *.c -o main -lm
	./main