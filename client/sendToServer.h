#ifndef SENDTOSERVER_H
#define SENDTOSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int startConnection(const int argc, const char *argv[]);
int stopConnection(const int sockfd);
int sendTo(const int sockfd, char *msg, const int length);
int receiveFrom(const int sockfd, char *msg, const int length);
#endif
