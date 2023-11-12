CC=gcc

CFLAGS=-Wall -Wextra -std=c89 -pedantic-errors

all:
	$(CC) geniuscrypt.c -o crypt $(CFLAGS)