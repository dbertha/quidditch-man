#ifndef CLIENT_H
#define CLIENT_H

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

class Client {
public:
	Client(int);
	void run();

private:
	int sockfd_; //socket d'écoute du serveur
	char msg[INPUTSIZE];
	char input_[50];
	char opponent_[10];
	fd_set FDSet_;
	enum Status {INIT,FREE,MATCH_LIST,MATCH_INVITING,MATCH_INVITED,MATCH_INGAME,DISCONNECTING};
	Status state_;

	int mainLoop();
	void loadFDSet();
	bool keyboard();
	void askInput();
	void login();
	void displayMenu();
	void kbMgr();
	void commMgr();
	void matchTentative();
	void contactServer();
	void receiveMessage();
};

#endif
