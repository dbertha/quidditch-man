#ifndef SERVER_H
#define SERVER_H

#include "Defines.hpp"
#include <iostream>
#include <set>
#include <vector>
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

class User;
class CommonMgr;
class Server {
public:
	Server(int);
	void run();
	int sendToClient(User *, char *, const int);
	std::vector<User*> GetUsersList();

private:
	CommonMgr* commonMgr_;
	int port_;
	int max_; //nombre de connections
	int sockfd_; //socket d'écoute du serveur
	struct sockaddr_in sockAddress_;
	int clientSockfd_;
	struct sockaddr_in clientSockAddress_;
	unsigned int sin_size;
	char msg[INPUTSIZE];
	fd_set FDSet_;
	std::vector<User*> usersList_;

	int connect();
	int mainLoop();
	void disconnect();
	void loadFDSet();
	bool isNewConnection();
	bool keyboard();
	int newUser();
	int receive(User *, char *, const int);
	void removeUser(int);
	void removeAllUsers();
};

#endif
