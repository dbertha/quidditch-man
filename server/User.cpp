#include "User.hpp"
#include "Server.hpp"
#include "CommonMgr.hpp"
#include "../common/NetworkBase.h"

User::User(Server * server, CommonMgr * commonMgr, int sockfd): server_(server), commonMgr_(commonMgr), sockfd_(sockfd), userId_(""), disconnecting_(false) {}

void User::cmdHandler(SerializedObject *received) {
	SerializedObject answer;
	char * position;
	int targetedBuilding;
	int targetedUser;
	int targetedPlayer;
	position = received->stringData;
#ifdef __DEBUG
	std::cout<<"En-tête reçu : "<<received->typeOfInfos<<std::endl;
#endif
	switch(received->typeOfInfos){
		//décodage du buffer en fonction de son entête
		//TODO : solution + élégante
		case LOGIN :
			//reading details
			char username[USERNAME_LENGTH], password[PASSWORD_LENGTH];
			position = received->stringData;
			memcpy(&username, position, sizeof(username)); //sizeof(char array[10]) = 10
			position += sizeof(username);
			memcpy(&password, position, sizeof(password));
#ifdef __DEBUG
			std::cout<<"Demande de login reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"Username reçu :  "<<username <<std::endl;
			std::cout<<"Password reçu :  "<<password <<std::endl;
#endif
			//handle demand
			
			//construct answer
			answer.typeOfInfos = LOGIN_CONFIRM;
			
			break;
		case GETBUILDINGINFOS :
			//reading details
			position = received->stringData;
			memcpy(&targetedBuilding, position, sizeof(targetedBuilding)); 
#ifdef __DEBUG
			std::cout<<"Demande d'infos de batiment reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"BuildingID reçu : "<<targetedBuilding<<std::endl;
			
#endif
			//handle demand
			
			//construct answer
			
			break;
		case UPGRADE_BUILDING :
			//reading details
			position = received->stringData;
			memcpy(&targetedBuilding, position, sizeof(targetedBuilding)); 
#ifdef __DEBUG
			std::cout<<"Demande d'infos d'upgrade de bâtiment reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"BuildingID reçu : "<<targetedBuilding<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case PROPOSEMATCH :
			//reading details
			position = received->stringData;
			memcpy(&targetedUser, position, sizeof(targetedUser)); 
#ifdef __DEBUG
			std::cout<<"Demande de proposition d'un match reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"userID reçu : "<<targetedUser<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case ACCEPTMATCH :
			//reading details
			position = received->stringData;
			memcpy(&targetedUser, position, sizeof(targetedUser)); 
#ifdef __DEBUG
			std::cout<<"Demande d'acceptation d'un match reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"userID reçu : "<<targetedUser<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case CREATEAUCTION :
			//reading details
			int startingPrice;
			position = received->stringData;
			memcpy(&targetedPlayer, position, sizeof(targetedPlayer)); 
			position += sizeof(targetedPlayer);
			memcpy(&startingPrice, position, sizeof(startingPrice)); 
#ifdef __DEBUG
			std::cout<<"Demande de création d'une enchère reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"playerID reçu : "<<targetedPlayer<<std::endl;
			std::cout<<"prix de départ reçu : "<<startingPrice<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case JOINAUCTION :
			//reading details
			int targetedAuction;
			position = received->stringData;
			memcpy(&targetedAuction, position, sizeof(targetedAuction)); 
#ifdef __DEBUG
			std::cout<<"Demande de participation à une enchère reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"targetedAuction reçu : "<<targetedAuction<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case GETMANAGERSLIST :
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des joueurs reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case GETAUCTIONSLIST :
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des enchères reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case GETPOSITIONS :
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des positions sur le terrain reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case SELECTPLAYER :
			//reading details
			position = received->stringData;
			memcpy(&targetedPlayer, position, sizeof(targetedPlayer)); 
#ifdef __DEBUG
			std::cout<<"Sélection d'un joueur sur le terrain reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"targetedPlayer reçu : "<<targetedPlayer<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case TRAIN_PLAYER :
			//reading details
			int capacityToTrain;
			memcpy(&targetedPlayer, position, sizeof(targetedPlayer)); 
			position += sizeof(targetedPlayer);
			memcpy(&capacityToTrain, position, sizeof(capacityToTrain)); 
#ifdef __DEBUG
			std::cout<<"Demande d'un entrainement pour un joueur reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"targetedPlayer reçu : "<<targetedPlayer<<std::endl;
			std::cout<<"capacityToTrain reçu : "<<capacityToTrain<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case MAKEMOVES :
			//reading details
			//soit un joueur se déplace, soit un joueur fait se déplacer une balle, soit ne fait rien : nbmax de mouvements = nbtotal de mouvement = nbjoueurs = 7
			//structure d'un mouvement : int playerID, int diagDest, int lineDest
			int moves[7][3];
			int diagDest;
			int lineDest;
			for(int i = 0; i < 7; ++i){
				memcpy(&targetedPlayer, position, sizeof(targetedPlayer)); 
				position += sizeof(targetedPlayer);
				memcpy(&diagDest, position, sizeof(diagDest));
				position += sizeof(diagDest); 
				memcpy(&lineDest, position, sizeof(lineDest));
				position += sizeof(lineDest); 
				moves[i][0] = targetedPlayer;
				moves[i][1] = diagDest;
				moves[i][2] = lineDest;
			}
#ifdef __DEBUG
			std::cout<<"Liste de déplacements reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"1er mouvement reçu : playerID diagDestination lineDestination "<<moves[0][0] << " " <<moves[0][1]<< " " << moves[0][2]<< std::endl;
			std::cout<<"3eme mouvement reçu : playerID diagDestination lineDestination "<<moves[2][0] << " " <<moves[2][1]<< " " << moves[2][2]<< std::endl;
			std::cout<<"7eme mouvement reçu : playerID diagDestination lineDestination "<<moves[6][0] << " " <<moves[6][1]<< " " << moves[6][2]<< std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case GETCURRENTPRICE :
			//no details to read : possible improvement : participation to several auctions at a same time
			//handle demand
			//construct answer
			break;
		case BID :
			//no details to read : possible improvement : participation to several auctions at a same time
			//construct answer
			break;
		default :
#ifdef __DEBUG
			std::cout<<"En-tête inconnu : "<<received->typeOfInfos<<std::endl;
#endif
			break;
		
	}
	//server_->sendToClient(this,&answer);
	//envoi de la réponse
}

void User::setDisconnection() {
	//ajouter ici les actions à prendre en cas de déconnection
	disconnecting_=true;
}

bool User::isDisconnecting() {return disconnecting_;}

int User::getSockfd() {return sockfd_;}

std::string User::getUserId() {return userId_;}
