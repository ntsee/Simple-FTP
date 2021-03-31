/**
 * Implementation of the functions defined in util.h
**/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "util.h"

#define MAX_PENDING 1

int InitializeServer(unsigned short port)
{
    /* Create socket for incoming connections */
    int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    struct sockaddr_in servAddr; /* Local address */
    memset(&servAddr, 0, sizeof(servAddr)); /* Zero out structure */
	servAddr.sin_family = AF_INET; /* Internet address family */
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	servAddr.sin_port = htons(port); /* Local port */

    /* Bind to the local address */
	if (bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
		DieWithError ( "bind () failed");

	/* Mark the socket so it will listen for incoming connections */
	if (listen(sockfd, MAX_PENDING) < 0)
		DieWithError("listen() failed");

    return sockfd;        
}

int AcceptClient(int serverSock)
{
    struct sockaddr_in clntAddr;
    int clntLen = sizeof(clntAddr);
    int clntSock = accept(serverSock, (struct sockaddr *) &clntAddr, &clntLen);
    if (clntSock < 0)
        DieWithError("accept() failed");

    return clntSock;
}

int InitializeClient(char* host, unsigned short port)
{
    /*Create a socket using TCP*/
    int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
        DieWithError("socket() failed");

    /*Construct the server address structure*/
    struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(host);
	servAddr.sin_port = htons(port);

    /*Establish connection to echo server*/
	if (connect(sockfd, (struct scokaddr*) &servAddr, sizeof (servAddr)) < 0)
		DieWithError("connect() failed");

    return sockfd;
}

void Write(int sock, void* buffer, int length)
{
    int sBytes, sv; 
    for (sBytes=0; sBytes<length; sBytes += sv)
    {
        if  ((sv = send(sock, buffer+sBytes, length-sBytes, 0)) <  0)
            DieWithError("send() failed");
    }
}

void WritePacket(int sock, struct Header* header, char* data)
{
    struct Header encodedHeader;
    encodedHeader.count = htons(header->count);
    encodedHeader.sequence = htons(header->sequence);
    Write(sock, (void*)&encodedHeader, sizeof(header));
    Write(sock, (void*)data, header->count);
}

void Read(int sock, void* buffer, int length)
{
    recv(sock, buffer, length, 0);
}

void ReadPacket(int sock, struct Header* header, char* data)
{
    Read(sock, (void*)header, sizeof(header));
    header->count = ntohs(header->count);
    header->sequence = ntohs(header->sequence);
    if (header->count)
        Read(sock, (void*)data, header->count);
}

void WaitForClose(int sock)
{
    char buffer[1];
    while (!recv(sock, buffer, sizeof(buffer), 0)) {}
}

void DieWithError(char* error)
{
    perror(error);
    exit(1);
}
