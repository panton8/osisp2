CC = gcc
CFLAGS = -Wall -pthread

all: multithreaded_sort

multithreaded_sort: multithreaded_sort.c
	$(CC) $(CFLAGS) multithreaded_sort.c -o multithreaded_sort

clean:
	rm -f multithreaded_sort