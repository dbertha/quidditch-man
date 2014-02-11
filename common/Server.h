#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Server {
public:
	Server(int);
	void run();

private:
	int port_;
	int max_; //nombre de connections
	int sockfd_; //socket d'écoute du serveur
	struct sockaddr_in sockAddress_;
	unsigned int sin_size;

	int connect();
	void disconnect();
};

#endif
