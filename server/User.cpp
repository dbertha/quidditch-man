#include "User.hpp"
#include "Server.hpp"
#include "CommonMgr.hpp"
#include "../common/NetworkBase.h"

User::User(Server * server, CommonMgr * commonMgr, int sockfd): server_(server), commonMgr_(commonMgr), sockfd_(sockfd), userId_(""), disconnecting_(false) {}

void User::cmdHandler(const SerializedObject *received) {
	SerializedObject answer;
	//traitement du message et
	switch(received->typeOfInfos){
		//TODO : solution + élégante
		case LOGIN :
			//handle demand
			//construct answer
			answer.typeOfInfos = LOGIN_CONFIRM;
			break;
		case GETBUILDINGINFOS :
			//handle demand
			//construct answer
			break;
		case UPGRADE_BUILDING :
			//handle demand
			//construct answer
			break;
		case PROPOSEMATCH :
			//handle demand
			//construct answer
			break;
		case ACCEPTMATCH :
			//handle demand
			//construct answer
			break;
		case CREATEAUCTION :
			//handle demand
			//construct answer
			break;
		case JOINAUCTION :
			//handle demand
			//construct answer
			break;
		case GETMANAGERSLIST :
			//handle demand
			//construct answer
			break;
		case GETAUCTIONSLIST :
			//handle demand
			//construct answer
			break;
		case GETPOSITIONS :
			//handle demand
			//construct answer
			break;
		case SELECTPLAYER :
			//handle demand
			//construct answer
			break;
		case MAKEMOVES :
			//handle demand
			//construct answer
			break;
		case GETCURRENTPRICE :
			//handle demand
			//construct answer
			break;
		case BID :
			//handle demand
			//construct answer
			break;
	}
	server_->sendToClient(this,&answer);
	//envoi de la réponse
}

void User::setDisconnection() {
	//ajouter ici les actions à prendre en cas de déconnection
	disconnecting_=true;
}

bool User::isDisconnecting() {return disconnecting_;}

int User::getSockfd() {return sockfd_;}

std::string User::getUserId() {return userId_;}
