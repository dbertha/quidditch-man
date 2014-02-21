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
#include "../server/Calendar.hpp"
class Server;
class MatchesHandler;
class User {
public:
	User(Server *, MatchesHandler *, int);
	void cmdHandler(SerializedObject *);
	void setDisconnection();
	bool isDisconnecting();
	int getSockfd();
	int getUserId();
	std::string getUserName();// initialement dans UserId...!
	//int sendAnswer(User *, const char cmd, std::string); cf NetworkBase.cpp
	void* createMatch();
	enum Status {INIT,FREE/*,MATCH_LIST*/,MATCH_INVITING,MATCH_INVITED,MATCH_INGAME,DISCONNECTING};
//	Status getState();
	Status state_; //TODO : getter/setter public
	pthread_t thread; // thread utilisé pour la gestion d'un match ou de tout autre activité spécifique

	int checkLoginAndPassword(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
	void addManager(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
	Manager* getManager();

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
};

#endif
