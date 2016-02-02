#Makefile for xwebserver

GCC = gcc
CFLAGS = -Wall -g -std=c99

all:
	$(GCC) $(CFLAGS) -o xwbserver.o main.c utils.c http.c
clean:
	rm -rf xwbserver
