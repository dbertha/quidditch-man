#ifndef USER_H
#define USER_H

#include <iostream>
class Server;
class User {
public:
	User(Server *, int);
	void cmdHandler(char *, const int);
	void setDisconnection();
	bool isDisconnecting();
	int getSockfd();
	std::string getUserId();

private:
	Server* server_;
	int sockfd_; //socket de communication du client
	std::string userId_;
	bool disconnecting_;
};

#endif
