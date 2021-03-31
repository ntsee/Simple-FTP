/**
 * This file contains utility functions for working with TCP sockets.
**/
#define MAX_LINE_SIZE 80

struct Header 
{
	unsigned short count;
	unsigned short sequence;
};

/**
* Creates and listens to a server socket at the given port
* Returns the sock fd
**/
int InitializeServer(unsigned short port);

/**
* Accepts a client connecting to the server
* Returns the sockFd of the client
**/
int AcceptClient(int sock);

/**
* Creates a client socket and connects to a server at the given port
* Returns the sock fd
*/
int InitializeClient(char* host, unsigned short port);

/**
* Writes length bytes of the buffer to the socket 
**/
void Write(int sock, void* buffer, int length);

/**
* Writes the header and data to the socket
**/
void WritePacket(int sock, struct Header* header, char* data);

/**
* Fills the buffer with length bytes from the socket
**/
void Read(int sock, void* buffer, int length);

/**
* Fills the buffheader and data with data from the socket
**/
void ReadPacket(int sock, struct Header* header, char* data);

/**
* Reads the socket until the stream closes
**/
void WaitForClose(int sock);

/**
* Outputs error and terminates program.
**/
void DieWithError(char* error);
