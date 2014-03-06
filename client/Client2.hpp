#ifndef CLIENT_H
#define CLIENT_H

#include "../common/Defines.hpp"
#include "../common/NetworkBase.hpp"
#include <iostream>
#include <set>
#include <vector>
#include <string> 
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
    //SerializedObject received;
	//char msg[INPUTSIZE];
	int input_;
	char opponent_[10];
	fd_set FDSet_;
	enum Status {INIT,FREE,MATCH_LIST,MATCH_INVITING,MATCH_INVITED,MATCH_INGAME,DISCONNECTING};
	Status state_;

	int mainLoop();
	void loadFDSet();
	bool keyboard();
	void askInput();
	void login();
	void displayMainMenu();
    void displayConnexionMenu();
    void displayManagerInfos();
    void displayPlayersList()
    void displayBuildingInfos(std::vector<int> buildingInfos, int buildingID);
    void displayPlayerInfos(vector<int> playerInfos, int playerID);
    std::string displayAuctionInfos(vector<string> auctionsList,vector<int> playerInfos, int auctionID);
    void displayAuctionMenus();
    void displaySellPlayerMenu();
    void displayManagePlayersMenu();
    void displayManageBuildingsMenu();
	void kbMgr();
	void commMgr();
	void matchTentative();
	void contactServer();
	void receiveMessage();
};

#endif
