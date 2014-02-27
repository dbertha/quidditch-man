#ifndef USER_H
#define USER_H
#include "../common/NetworkBase.h"
#include "MatchesHandler.hpp"
#include <iostream>
#include <pthread.h>
#include <string>
#include <sstream>
#include <iostream>
#include "../server/Manager.hpp"
#include "../server/ManagedPlayer.hpp"
#include "../server/Calendar.hpp"


class Server;
class Auction;

#define INIT 0
#define FREE 1 
#define MATCH_INVITING 2
#define MATCH_INVITED 3
#define MATCH_INGAME 4
#define DISCONNECTING 5

class MatchesHandler;
class User {
public:
	User(Server *, MatchesHandler *, int, int);
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

	std::string intToString(int value);

private:
	Server* server_;
	//CommonMgr* commonMgr_;  inutilisé
    MatchesHandler *__matchesHandler;
	//User* opponent_;
	int sockfd_; //socket de communication du client
	int userId_;
	std::string userName_;
	std::string answer_; //TODO : inutilisé
	std::string dataRequest_;
	Manager* manager_;
	Calendar* calendar_;
	Auction* auction_;
};

#endif
