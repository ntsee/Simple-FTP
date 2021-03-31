List of Relevant Files
- makefile
- client.c
- server.c
- util.h
- util.c

Compilation Instructions
- Run "make" in the project directory to build both server and client.
- Run "make server" to build the server.
- Run "make client" to build the client.

Configuration file(s)
- N/A

Running Instructions
- The "server" executable starts up a TCP server that listens on the specified port. The server terminates after serving one client.
    - Usage: ./server <Server Port>
- The "client" executable starts up a TCP client that connects to the specified IP address and port number. A file with the specified name is requested and transferred to the client. This file is saved as 'out.txt'.
    - Usage: ./client <Server IP> <Server Port> <File Name>

