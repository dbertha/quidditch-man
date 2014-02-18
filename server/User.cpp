#include "User.hpp"
#include "Server.hpp"
#include "CommonMgr.hpp"
User::User(Server * server, CommonMgr * commonMgr, int sockfd): server_(server), commonMgr_(commonMgr), sockfd_(sockfd), userId_(""), disconnecting_(false) {}

void User::cmdHandler(char * msg, const int length) {
//traitement du message et
	server_->sendToClient(this,msg,length);
	//envoi de la réponse
}

void User::setDisconnection() {
	//ajouter ici les actions à prendre en cas de déconnection
	disconnecting_=true;
}

bool User::isDisconnecting() {return disconnecting_;}

int User::getSockfd() {return sockfd_;}

std::string User::getUserId() {return userId_;}
