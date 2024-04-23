#ifndef _COMM_H_
#define _COMM_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int serverSocket(unsigned int addr, int port, int type, char ip_out[INET_ADDRSTRLEN]);
int serverAccept(int sd);
int clientSocket(char *remote, int port);
int closeSocket(int sd);

int sendMessage(int socket, char *buffer, int len);
int recvMessage(int socket, char *buffer, int len);

ssize_t writeLine(int fd, char *buffer);
ssize_t readLine(int fd, char *buffer, size_t n);

#endif