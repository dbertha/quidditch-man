#ifndef USER_H
#define USER_H

#include "../common/NetworkBase.hpp"

#include "Manager.hpp"
#include "ManagedPlayer.hpp"
#include "Calendar.hpp"



#include <string> 
#include <sys/stat.h>
#include <iostream>
#include <stdlib.h>     /* atoi */
#include <ctime>
#include <cstdlib> 
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

#include <pthread.h>
#include <sstream>

//TODO : handleDisconnection() with testing state_

class Server;
class Auction;
class MatchesHandler;

#define INIT 0
#define FREE 1 
#define MATCH_INVITING 2
#define MATCH_INVITED 3
#define MATCH_INGAME 4
#define DISCONNECTING 5
#define ADMIN 6


class User {
public:
	User(Server *, MatchesHandler *, int, int);
	~User();
	void cmdHandler(SerializedObject *);
	void setDisconnection();
	bool isDisconnecting();
	int getSockfd();
	int getUserID();
	std::string getUserName();// initialement dans UserId...!
	//int sendAnswer(User *, const char cmd, std::string); cf NetworkBase.cpp
	void* createMatch();
	
//	Status getState();
	int state_; //TODO : getter/setter public
	pthread_t thread; // thread utilisé pour la gestion d'un match ou de tout autre activité spécifique

	int checkLoginAndPassword(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
	void addManager(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
	Manager* getManager();

	void auctionWin(Manager* manager, ManagedPlayer player);
	void handleEndOfMatch(int numTeam, int numWinningTeam);
	
	void handleEndOfMatch(int numTeam, int numWinningTeam, int tournamentPrice, std::vector<int> lifes); //normal price + tournamentPrice
	void handleEndOfTrainingMatch(int numTeam, int numWinningTeam, std::vector<int> lifes);

	std::string intToString(int value);

private:
	Server* server_;
    MatchesHandler *__matchesHandler;
	//User* opponent_;
	int sockfd_; //socket de communication du client
	int userId_;
	std::string userName_;
	std::string answer_; //TODO : inutilisé
	std::string dataRequest_;
	std::vector<ManagedPlayer *> _teamInMatch;
	Manager* manager_;
	Calendar* calendar_;
	Auction* auction_;
    int **__moves;
    struct tm timeOfStart_;
    bool isAboutManagement(SerializedObject &received){ return (received.typeOfInfos <= MANAGEMENTINDEXEND) and (received.typeOfInfos >= MANAGEMENTINDEXSTART);}
    bool isAboutMatch(SerializedObject &received){ return (received.typeOfInfos <= MATCHESINDEXEND) and (received.typeOfInfos >= MATCHESINDEXSTART);}
    bool isAboutAuctions(SerializedObject &received){ return (received.typeOfInfos <= AUCTIONSINDEXEND) and (received.typeOfInfos >= AUCTIONSINDEXSTART);}
    void handleManagementRequest(SerializedObject &received);
    void handleMatchRequest(SerializedObject &received);
    void handleAuctionRequest(SerializedObject &received);
};

#endif
