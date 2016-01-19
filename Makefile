#Makefile for xwebserver

GCC = gcc
CFLAGS = -Wall -g -std=c99

all:
	$(GCC) $(CFLAGS) -o main.o main.c utils.c
clean:
	rm -rf *.o
