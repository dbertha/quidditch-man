#ifndef USER_H
#define USER_H
#include <pthread.h>
#include <string>
#include <sstream>
#include <iostream>
class User;
struct threadData {
	User * host;
	User * opponent;
};
class Server;
class CommonMgr;
class User {
public:
	User(Server *, CommonMgr *, int);
	int cmdHandler(char *, const int);
	void setDisconnection();
	bool isDisconnecting();
	int getSockfd();
	std::string getUserId();
	int sendAnswer(User *, const char cmd, std::string);
//	void* createMatch();
	enum Status {INIT,FREE,MATCH_LIST,MATCH_INVITING,MATCH_INVITED,MATCH_INGAME,DISCONNECTING};
//	Status getState();
	Status state_;
	pthread_t thread; // thread utilisé pour la gestion d'un match ou de tout autre activité spécifique

private:
	Server* server_;
	CommonMgr* commonMgr_;
	User* opponent_;
	int sockfd_; //socket de communication du client
	std::string userId_;
	std::string answer_;
	std::string dataRequest_;
	struct threadData tData;
};

#endif
