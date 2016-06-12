#Makefile for xwebserver

GCC = gcc
CFLAGS = -Wall -g -std=c99
LIBS= -pthread
IN= main.c utils.c http.c
OUT= xwbserver


all:
	$(GCC) $(CFLAGS) -o $(OUT) $(IN) $(LIBS)
clean:
	rm -rf $(OUT)
