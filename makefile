CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb

all: lp

lp: lp.c
	$(CC) $(CFLAGS) -o lp.exe lp.c
	
