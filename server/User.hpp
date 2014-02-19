#ifndef USER_H
#define USER_H
#include "../common/NetworkBase.h"
#include <iostream>
<<<<<<< HEAD
#include "../common/Manager.hpp"
=======
#include <pthread.h>
#include <string>
#include <sstream>
#include <iostream>
>>>>>>> 505aa8608c1c738b3699abb8d61788a974a6c271
class Server;
class CommonMgr;
class User {
public:
	User(Server *, CommonMgr *, int);
	void cmdHandler(SerializedObject *);
	void setDisconnection();
	bool isDisconnecting();
	int getSockfd();
	std::string getUserId();
	//int sendAnswer(User *, const char cmd, std::string); cf NetworkBase.cpp
	void* createMatch();
	enum Status {INIT,FREE,MATCH_LIST,MATCH_INVITING,MATCH_INVITED,MATCH_INGAME,DISCONNECTING};
//	Status getState();
	Status state_;
	pthread_t thread; // thread utilisé pour la gestion d'un match ou de tout autre activité spécifique

private:
	Manager& manager_;
	Server* server_;
	CommonMgr* commonMgr_;
	User* opponent_;
	int sockfd_; //socket de communication du client
	std::string userId_;
<<<<<<< HEAD
	bool disconnecting_;
	bool checkLoginAndPassword(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
=======
	//std::string answer_;
	std::string dataRequest_;
>>>>>>> 505aa8608c1c738b3699abb8d61788a974a6c271
};

#endif
