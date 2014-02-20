#include "User.hpp"
#include "Server.hpp"
#include "CommonMgr.hpp"
#include "../common/NetworkBase.h"
#include "../server/Manager.hpp"

#include <sys/stat.h>
#include <iostream>
#include <stdlib.h>     /* atoi */
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

User::User(Server * server, CommonMgr * commonMgr, int sockfd): server_(server), commonMgr_(commonMgr), sockfd_(sockfd), state_(INIT), userId_(0), userName_(""), manager_(NULL) {}
//TODO : initialisation dans le bon ordre
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
			//handle demand:
			if (checkLoginAndPassword(username,password)) {
				std::cout<<"LOGIN OK"<<std::endl;
				manager_ = new Manager(username);
			}
			else std::cout<<"WRONG LOGIN/PASSWORD"<<std::endl;
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
		case PROPOSEMATCH : {
			//reading details
			position = received->stringData;
			memcpy(&targetedUser, position, sizeof(targetedUser));
#ifdef __DEBUG
			std::cout<<"Demande de proposition d'un match reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"userID reçu : "<<targetedUser<<std::endl;
#endif
						for (unsigned int i=0;i<server_->usersList_.size();++i)
							if(server_->usersList_[i]->state_==FREE && server_->usersList_[i]->getUserId()==targetedUser) {
								opponent_=server_->usersList_[i];
								opponent_->state_=MATCH_INVITED;
								state_=MATCH_INVITING;
								answer_+="Le manager "+server_->usersList_[i]->getUserName()+" vous invite à jouer un match amical.";
//								return sendAnswer(server_->usersList_[i],msg[0],answer_);
//					TODO		à envoyer au manager invité !
							}
						answer_="0Vous n'avez pas sélectionné un joueur disponible.";
			break;
			}
		case ACCEPTMATCH : {
			//reading details
			int confirmation; //temporary for gcc compiler (testing only). Change to bool when compiled with g++
			position = received->stringData;
			memcpy(&targetedUser, position, sizeof(targetedUser));
			position += sizeof(targetedUser);
			memcpy(&confirmation, position, sizeof(confirmation));
#ifdef __DEBUG
			std::cout<<"Demande d'acceptation d'un match reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"userID reçu : "<<targetedUser<<std::endl;
			std::cout<<"confirmation : "<<confirmation<<std::endl;
#endif
			std::string answer_;
			if (confirmation) {
						std::cout<<userId_<<" est d'accord pour rencontrer "<<targetedUser<<" en match amical"<<std::endl;
						for (unsigned int i=0;i<server_->usersList_.size();++i)
							if(server_->usersList_[i]->state_==MATCH_INVITING && server_->usersList_[i]->getUserId()==targetedUser) {
								server_->usersList_[i]->state_=MATCH_INGAME;
								state_=MATCH_INGAME;
								answer_=" Ce joueur est d'accord !";
//			TODO		message à envoyer au manager qui invite :
//								return sendAnswer(server_->usersList_[i],msg[0],answer_);
							}
						state_=FREE;
						answer_="0Le joueur qui vous invitait n'est plus disponible !";
					}
			else {
						answer_="0Ce joueur n'est pas d'accord.";
						for (unsigned int i=0;i<server_->usersList_.size();++i)
							if(server_->usersList_[i]->getUserId()==targetedUser) {
								server_->usersList_[i]->state_=FREE;
								state_=FREE;
							}
			}
			break;
		}
		case IS_MATCH_WAITING ://no more needed
			//no details to read
			//on suppose qu'un seul match sera demandé à la fois
#ifdef __DEBUG
			std::cout<<"Demande si match demandé reçue sur socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case CREATEAUCTION ://
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
		case GETMANAGERSLIST : {
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des joueurs reçue sur le socket "<<getSockfd()<<std::endl;
#endif
					std::string answer_;
					std::ostringstream oSStream_;
					if(state_!=FREE) {
						answer_="0Internal error : user already busy in a conversation with the server.";
					}
					else {
						state_=MATCH_LIST;
						answer_=' ';
						for (unsigned int i=0;i<server_->usersList_.size();++i)
							if(server_->usersList_[i]->state_==FREE) {
								oSStream_<<server_->usersList_[i]->userId_;
								answer_=answer_+oSStream_.str()+" "+server_->usersList_[i]->userName_+"\n";
								state_=MATCH_LIST;
							}
						if(answer_.length()==1) answer_="0Il n'y a pas d'autres managers disponibles.";
					}
			break;
			}
		case GETAUCTIONSLIST :
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des enchères reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case GETPOSITIONS ://
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des positions sur le terrain reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			//construct answer
			break;
		case SELECTPLAYER ://
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
	state_=DISCONNECTING;
}

bool User::isDisconnecting() {return (state_==DISCONNECTING);}
int User::getSockfd() {return sockfd_;}

int User::getUserId() {return userId_;}

std::string User::getUserName() {return userName_;}

bool User::checkLoginAndPassword(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]) {
	int fd = open("server/Saves/managers.txt",O_RDONLY);
	if (fd==-1){
		std::cerr<<"Error while opening file\n";
		return 0;
	}

	int size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);

	char buffer[size+1];

	int byte = read(fd,buffer,size);
	buffer[byte]='\0';

	close(fd);

	char *line, *login, *pass, *context1, *context2;

	if ( (line=strtok_r(buffer,"\n",&context1))!=NULL ) {
		do {
			login = strtok_r(line,"#",&context2);
			pass = strtok_r(NULL,"#",&context2);
			if ( (strcmp(login,username)==0)&&(strcmp(pass,password)==0) ) {
				userName_=username;
				userId_=server_->getNewUserId();
				return 1;
			}
		} while ( (line=strtok_r(NULL,"\n",&context1))!=NULL ) ;
	}

	return 0;
}
