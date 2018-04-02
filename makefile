CC=gcc
CFLAGS=-Wall -Wextra -std=c11

output: main.o tree.o
	$(CC) main.o tree.o -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
	
tree.o: tree.h tree.c
	$(CC) $(CFLAGS) -c tree.c

clean: 
	rm *.o main
