/**
 * Progam that serves as a TCP client for transfering files following the protocol specified in 
 * CISC 450 Programming Project 1
*/
#include <stdio.h> /*for printf() and fprintf()*/
#include <sys/socket.h> /*for socket(), connect(), send(), and recv()*/
#include <arpa/inet.h> /*for sockaddr_in and inet_addr()*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s <Server IP> <Server Port> <File Name>\n", argv[0]);
		exit(1);
	}
	
	int packets = 0;
	int bytes = 0;
	FILE* file = fopen("out.txt", "w");
	if (!file)
		DieWithError("Could not make out.txt");

	struct Header header;
	char data[MAX_LINE_SIZE + 1];
	header.count = strlen(argv[3]);
	header.sequence = 0;
	int sockFd = InitializeClient(argv[1], atoi(argv[2]));
	WritePacket(sockFd, &header, argv[3]);

	ReadPacket(sockFd, &header, data);
	while (header.count != 0)
	{
		packets++;
		bytes += header.count;
		printf("Data Received: '%s'", data);
		printf("Packet %d received with %d data bytes\n", header.sequence, header.count);
		if (!fputs(data, file))
			DieWithError("fputs() failed");
		
		ReadPacket(sockFd, &header, data);
	}

	printf("End of Transmission Packet with sequence number %d received with %d data bytes\n", header.sequence, header.count);
	printf("%d data packets received\n", packets);
	printf("%d data bytes received\n", bytes);
	fclose(file);
	close(sockFd);
	exit(0);
}
