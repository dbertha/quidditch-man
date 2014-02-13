#include "User.h"

User::User(int sockfd): sockfd_(sockfd), userId_(""), disconnecting_(false) {}

void User::setDisconnection() {
	//ajouter ici les actions à prendre en cas de déconnection
	disconnecting_=true;
}

bool User::isDisconnecting() {return disconnecting_;}

int User::getSockfd() {return sockfd_;}

std::string User::getUserId() {return userId_;}
