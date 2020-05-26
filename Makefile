CC = gcc
DEBUGFLAGS = -Wall
CFLAGS = -D_REENTRANT $(DEBUGFLAGS) -D_XOPEN_SOURCE=500
LDFLAGS = -lpthread

all: server control

serv:
	$(CC) server.c -o server

control:
	$(CC) control.c -o control

clean:
	rm -f server control
