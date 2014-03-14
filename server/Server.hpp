#ifndef SERVER_H
#define SERVER_H

#include "../common/Defines.hpp"
#include "../common/NetworkBase.hpp"
#include "MatchesHandler.hpp"
#include "User.hpp"
#include "Auction.hpp"
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
#include <string>

class User;
class Auction;
class ManagedPlayer;
class MatchesHandler;
class Server {
public:
	Server(int);
	void run();
	int sendToClient(User *, SerializedObject *); //TODO : transformer la modélisation pour conserver en private ou protected
	std::vector<User*> usersList_;
	std::vector<User*> GetUsersList();
	std::vector<Auction*> auctionsList_;
	std::vector<Auction*> getAuctionsList();
	int getAuctionID(int index);
	std::vector<int> getPlayerSoldInfos(int index);
	std::string getPlayerSoldName(int index);
	int getAuctionTimeLeft(int index);
	int getAuctionStartingPrice(int index);
	void createAuction(User* user, ManagedPlayer player, int startingPrice);
	//int getNewUserId();

private:
	MatchesHandler __matchesHandler;
	int port_;
	int max_; //nombre de connections
	int sockfd_; //socket d'écoute du serveur
	int clientSockfd_;
	struct sockaddr_in clientSockAddress_;
	unsigned int sin_size;
	char msg[INPUTSIZE];
	fd_set FDSet_;
	int _nextuserID;

	int connect();
	int mainLoop();
	void disconnect();
	void loadFDSet();
	bool isNewConnection();
	bool keyboard();
	int newUser();
	int receive(User *, SerializedObject *);
	void removeUser(int);
	void removeAllUsers();
};

#endif
