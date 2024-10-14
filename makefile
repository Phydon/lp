CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb

all: lp

lp: lp.c
	$(CC) -o lp.exe lp.c

test: 
	$(CC) -o test.out lp.c
	
clean: 
	rm lp.exe
