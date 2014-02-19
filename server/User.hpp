#ifndef USER_H
#define USER_H
#include "../common/NetworkBase.h"
#include <iostream>
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

private:
	Server* server_;
	CommonMgr* commonMgr_;
	int sockfd_; //socket de communication du client
	std::string userId_;
	bool disconnecting_;
};

#endif
