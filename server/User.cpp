#include "User.hpp"
#include "Server.hpp"
#include "CommonMgr.hpp"
#include "../common/NetworkBase.h"
#include "../server/Manager.hpp"
#include "../server/Calendar.hpp"

#include <sys/stat.h>
#include <iostream>
#include <stdlib.h>     /* atoi */
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>




User::User(Server * server, CommonMgr * commonMgr, int sockfd): server_(server), commonMgr_(commonMgr), sockfd_(sockfd), state_(INIT), userId_(0), manager_(NULL), calendar_(NULL) {}
//TODO : initialisation dans le bon ordre
void User::cmdHandler(SerializedObject *received) {
	SerializedObject answer;
	char * position;
	char * answerPosition = answer.stringData;
	int targetedBuilding;
	int targetedUser;
	int targetedPlayer;
	int resultOfUpgrade;
	bool resultOfTraining;
	bool confirmation;
	vector<int> infos;
	vector<string> playersList;
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
			if (checkLoginAndPassword(username,password) == 1) {
				std::cout<<"LOGIN OK"<<std::endl;
				manager_ = new Manager(username);
				userName_=username;
				userId_=server_->usersList_.size();
				calendar_ = new Calendar(manager_);
				calendar_->update();
				manager_->save();
				state_=FREE;
				confirmation = true;
			}
			else {
				std::cout<<"WRONG LOGIN/PASSWORD"<<std::endl;
				confirmation = false;
			}
			//construct answer
			answer.typeOfInfos = LOGIN_CONFIRM;

			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			
			break;

		case CREATE_MANAGER :
			//reading details
			//char username[USERNAME_LENGTH], password[PASSWORD_LENGTH];
			position = received->stringData;
			memcpy(&username, position, sizeof(username)); //sizeof(char array[10]) = 10
			position += sizeof(username);
			memcpy(&password, position, sizeof(password));
#ifdef __DEBUG
			std::cout<<"Nouveau login reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"Username reçu :  "<<username <<std::endl;
			std::cout<<"Password reçu :  "<<password <<std::endl;
#endif
			//handle demand:
			confirmation = false;
			if ((checkLoginAndPassword(username,password)==-1)||(checkLoginAndPassword(username,password)==1)) { //This Manager login already exists
				std::cout<<"LOGIN ALREADY TAKEN"<<std::endl;
			}
			else {
				std::cout<<"CREATING MANAGER..."<<std::endl;
				addManager(username,password);
				manager_ = new Manager(username);
				userName_=username;
				userId_=server_->usersList_.size();
				manager_->save();
				calendar_ = new Calendar(manager_);

				state_=FREE;
				confirmation = true;
			}
			//construct answer
			answer.typeOfInfos = LOGIN_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			break;

		case GETMANAGERINFOS :
			//no detail to read
#ifdef __DEBUG
			std::cout<<"Demande des infos du manager reçue sur socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			
			calendar_->update();
			manager_->save();
			int nbPlayers;
			nbPlayers = manager_->getNumberOfPlayers();
			int money;
			money = manager_->getMoney();
			int nbFans;
			nbFans = manager_->getNumberOfFans();
#ifdef __DEBUG
			std::cout<<"Number of players : "<<manager_->getNumberOfPlayers()<<std::endl;
			std::cout<<"Money : "<<manager_->getMoney()<<std::endl;
			std::cout<<"Number of fans : "<<manager_->getNumberOfFans()<<std::endl;
#endif
			calendar_->update();
			manager_->save();
			//construct answer
			answer.typeOfInfos = MANAGERINFOS;
			memcpy(answerPosition, &nbPlayers, sizeof(nbPlayers));
			answerPosition += sizeof(nbPlayers);
			memcpy(answerPosition, &money, sizeof(money));
			answerPosition += sizeof(money);
			memcpy(answerPosition, &nbFans, sizeof(nbFans));
			sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			break;

		case GETPLAYERSLIST :
			//no detail to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des joueurs reçue sur socket "<<getSockfd()<<std::endl;
#endif
			//handle demand

			calendar_->update();
			manager_->save();
			playersList = manager_->getPlayersList();
			//construct answer 
			answer.typeOfInfos = PLAYERSLIST;
			int size;
			size = playersList.size();
			memcpy(answerPosition, &size, sizeof(size)); //nb de noms à lire
			answerPosition += sizeof(size);
			for (unsigned int i=0;i<playersList.size() - 1;i+=2) { //un paquet par joueur
				char playerFirstName[USERNAME_LENGTH], playerLastName[USERNAME_LENGTH];
				//un managedPlayer dans la liste =
				//indice dans la liste déductible par l'ordre en renvoi
				//nom : char[30]
				//prénom : char[30]
				//on suppose le buffer assez grand
				string firstname = playersList[i];
				string lastname = playersList[i+1];
				strcpy(playerFirstName, firstname.c_str());
				strcpy(playerLastName, lastname.c_str());
				memcpy(answerPosition, &playerFirstName, sizeof(playerFirstName));
				answerPosition += sizeof(playerFirstName);
				memcpy(answerPosition, &playerLastName, sizeof(playerLastName));
				answerPosition += sizeof(playerLastName);
				
			}
			calendar_->update();
			manager_->save();
			sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			
			break;

		case GETPLAYERINFOS :
			//reading details
			memcpy(&targetedPlayer, position, sizeof(targetedPlayer)); 
#ifdef __DEBUG
			std::cout<<"Demande d'informations pour un joueur reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"targetedPlayer reçu : "<<targetedPlayer<<std::endl;
#endif
			infos= manager_->getPlayerInformations(targetedPlayer);
			//for (unsigned int i=0;i<infos.size();++i) {std::cout<<infos[i]<<std::endl;}
			calendar_->update();
			manager_->save();
			//construct answer
			answer.typeOfInfos = PLAYERINFOS;
			//5 attributs int
			//5 états d'entrainements d'attribut int
			//1 int blocked
			//1 int bonus du balais
			//1 int capacity du balais
			//TODO : ajouter la vie
			std::cout << infos.size() << std::endl;
			for(unsigned int i = 0; i < infos.size(); ++i){
				int value;
				value = infos[i];
				memcpy(answerPosition, &value, sizeof(value));
				answerPosition += sizeof(value);
			}
			sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			
			break;

		case GETBUILDINGINFOS :
			//reading details
			position = received->stringData;
			memcpy(&targetedBuilding, position, sizeof(targetedBuilding)); 
#ifdef __DEBUG
			std::cout<<"Demande d'infos de batiment reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"BuildingID reçu : "<<targetedBuilding<<std::endl;

#endif
			//handle demand:
			calendar_->update();
			manager_->save();
			//RecrutmentCenter inutilisé
			if (targetedBuilding==STADIUM) infos=manager_->getStadiumInformations();
			else if (targetedBuilding==TRAININGCENTER) infos=manager_->getTrainingCenterInformations();
			else if (targetedBuilding==HOSPITAL) infos=manager_->getHospitalInformations();
			else /*if (targetedBuilding==FANSHOP)*/ infos=manager_->getFanShopInformations();
			

			calendar_->update();
			manager_->save();
	
			//construct answer

			answer.typeOfInfos = BUILDINGINFOS;
			for (unsigned int i=0;i<4;++i) {
				//std::cout<<infos[i]<<std::endl;
				int attr;
				attr = infos[i];
				memcpy(answerPosition, &attr, sizeof(attr));
				answerPosition += sizeof(attr);
			}
			sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			break;
		case UPGRADE_BUILDING :
			//reading details
			position = received->stringData;
			memcpy(&targetedBuilding, position, sizeof(targetedBuilding)); 
#ifdef __DEBUG
			std::cout<<"Demande d'infos d'upgrade de bâtiment reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"BuildingID reçu : "<<targetedBuilding<<std::endl;
#endif
			//handle demand:
			calendar_->update();
			manager_->save();
			confirmation = false;
			if (targetedBuilding==STADIUM) resultOfUpgrade=manager_->startStadiumConstruction();
			else if (targetedBuilding==TRAININGCENTER) resultOfUpgrade=manager_->startTrainingCenterConstruction();
			else if (targetedBuilding==HOSPITAL) resultOfUpgrade=manager_->startHospitalConstruction();
			else if (targetedBuilding==FANSHOP) resultOfUpgrade=manager_->startFanShopConstruction();
			if (resultOfUpgrade==ALREADYINCONSTRUCTION) std::cout<<"Already in construction !"<<std::endl;
			else if (resultOfUpgrade==NOTENOUGHMONEY) std::cout<<"Not enough money !"<<std::endl; //testé au niveau client
			else {
				std::cout<<"Construction started !"<<std::endl;
				confirmation = true;
			}
			calendar_->update();
			manager_->save();
			//construct answer
			answer.typeOfInfos = UPGRADE_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
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
			calendar_->update();
			manager_->save();
			confirmation = manager_->trainPlayer(targetedPlayer,capacityToTrain);
			if (!confirmation) std::cout<<"This player is already blocked by a training or the hospital"<<std::endl;
			calendar_->update();
			manager_->save();

			//construct answer
			answer.typeOfInfos = TRAINING_STARTED;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			break;	

		case HEAL_PLAYER :
			//reading details
			memcpy(&targetedPlayer, position, sizeof(targetedPlayer)); 
#ifdef __DEBUG
			std::cout<<"Demande de soin pour un joueur reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"targetedPlayer reçu : "<<targetedPlayer<<std::endl;
#endif
			//handle demand
			calendar_->update();
			manager_->save();
			confirmation = manager_->healPlayer(targetedPlayer);
			if (!confirmation) std::cout<<"This player is already blocked by a training or the hospital or is in full health"<<std::endl;
			calendar_->update();
			manager_->save();

			//construct answer
			answer.typeOfInfos = HEAL_STARTED;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			break;

		case PROPOSEMATCH : {
			//reading details
			position = received->stringData;

			memcpy(&targetedUser, position, sizeof(targetedUser));

			position += sizeof(targetedUser);
			std::vector<int> playersInTeam; //indice des ManagedPlayer à faire jouer
			for(int i = 0; i < 7; ++i){
				int value;
				memcpy(&value,position, sizeof(value));
				position += sizeof(value);
				playersInTeam.push_back(value); //ajout à la liste
			}
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
			bool confirmation; 
			position = received->stringData;
			memcpy(&targetedUser, position, sizeof(targetedUser));
			position += sizeof(targetedUser);

			memcpy(&confirmation, position, sizeof(confirmation));
			position += sizeof(confirmation);
			std::vector<int> playersInTeam; //indice des ManagedPlayer à faire jouer
			for(int i = 0; i < 7; ++i){
				int value;
				memcpy(&value,position, sizeof(value));
				position += sizeof(value);
				playersInTeam.push_back(value); //ajout à la liste
			}
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
		case GETMANAGERSLIST : {
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des managers reçue sur le socket "<<getSockfd()<<std::endl;
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
					std::cout<<answer_<<std::endl;

					state_=FREE;
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

int User::checkLoginAndPassword(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]) {
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
			if (strcmp(login,username)==0) {
 				if (strcmp(pass,password)==0) {
 					return 1;
 				}
				else return -1;
 			}
		} while ( (line=strtok_r(NULL,"\n",&context1))!=NULL ) ;
	}

	return 0;
}

void User::addManager(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]) {
	int fd = open("server/Saves/managers.txt",O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		std::cerr<<"Error while opening file\n";
		return;
	}
	string user,pass;
	user=username;
	pass=password;
	write(fd,user.c_str(),user.size());
	write(fd,"#",1);
	write(fd,pass.c_str(),pass.size());
	write(fd,"#",1);
	write(fd,"\n",1);
	close(fd);
}

Manager* User::getManager() {return manager_;}
