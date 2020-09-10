#!Makefile
CC = gcc
CFLAGS = -g -W -Wall -lpthread
DEBUG = -D DEBUG
SRC =  $(wildcard *.c)

all: httpd
httpd:
	$(CC) $(CFLAGS) $(SRC) -o httpd

debug: 
	$(CC) $(CFLAGS) $(SRC) $(DEBUG) -o httpd

clean:
	rm httpd
