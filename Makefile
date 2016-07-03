#Makefile for xwebserver

GCC = gcc
CFLAGS = -Wall -std=c99
LIBS= -pthread
IN= main.c utils.c http.c
OUT= xwbserver


all:
	$(GCC) $(CFLAGS) -o $(OUT) $(IN) $(LIBS)
debug:
	$(GCC) $(CFLAGS) -g -o $(OUT) $(IN) $(LIBS)
opt:
	$(GCC) $(CFLAGS) -O2 -o $(OUT) $(IN) $(LIBS)
clean:
	rm -rf $(OUT)
