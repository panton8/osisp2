CC = gcc
CFLAGS = -Wall

all: mydaemon

mydaemon: mydaemon.o
	$(CC) $(CFLAGS) mydaemon.o -o mydaemon

mydaemon.o: mydaemon.c
	$(CC) $(CFLAGS) -c mydaemon.c

clean:
	rm -f *.o mydaemon
