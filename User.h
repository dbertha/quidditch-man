#ifndef USER_H
#define USER_H

#include <iostream>
#include "Server.h"
class User {
public:
	User(int);
	void cmdHandler(Server *, char *, const int);
	void setDisconnection();
	bool isDisconnecting();
	int getSockfd();
	std::string getUserId();

private:
	int sockfd_; //socket de communication du client
	std::string userId_;
	bool disconnecting_;
};

#endif
