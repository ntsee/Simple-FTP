/**
 * Progam that serves as a TCP client for transfering files following the protocol specified in 
 * CISC 450 Programming Project 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int main(int argc, char *argv[])
{
	if (argc != 2) /* Test for correct number of arguments */
	{
		fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
		exit(1);
	}

	int serverSockFd = InitializeServer(atoi(argv[1]));
	int clientSockFd = AcceptClient(serverSockFd);
	
	struct Header header;
	char data[MAX_LINE_SIZE + 1];
	int sequence = 0;
	int packets = 0;
	int bytes = 0;

	ReadPacket(clientSockFd, &header, data);
	FILE* file = fopen(data, "r");
	if (!file)
		DieWithError("fopen() failed");

	while (fgets(data, sizeof(data), file))
	{
		header.count = strlen(data) + 1;
		header.sequence = sequence++;
		WritePacket(clientSockFd, &header, data);
		printf("Packet %d transmitted with %d data bytes\n", header.sequence, header.count);
		bytes += header.count;
		packets++;
	}

	header.count = 0;
	header.sequence = sequence++;
	WritePacket(clientSockFd, &header, "");
	printf("End of Transmission Packet with sequence number %d transmitted with %d data bytes\n", header.sequence, strlen(""));
	printf("%d data packets transmitted\n", packets);
	printf("%d data bytes rectransmitted\n", bytes);
	fclose(file);

	WaitForClose(clientSockFd);
	close(serverSockFd);
}