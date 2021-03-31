CC=gcc
CFLAGS=-Wall

all: server client

OTHER_OBSS = util.o

server: server.o $(OTHER_OBSS)

client: client.o $(OTHER_OBSS)

util.o: util.c util.h
	$(CC) -c util.c

server.o: server.c
	$(CC) -c server.c

client.o: client.c
	$(CC) -c client.c

clean:
	rm -f server client server.o client.o util.o out.txt
