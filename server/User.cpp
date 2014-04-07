#include "User.hpp"

#include "MatchesHandler.hpp" 
#include "Auction.hpp"
#include "Server.hpp" //TODO : supprimer cette dépendance
#include "DataBase.hpp"
	
#include <string> 
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



User::User(Server * server, MatchesHandler *matchesHandler, int sockfd, int userID): state_(INIT), server_(server), __matchesHandler(matchesHandler), sockfd_(sockfd), userId_(userID), manager_(NULL), calendar_(NULL), auction_(NULL) {
    //username vide
    __moves = new int*[7]; //7 lignes
    for(int i =0; i < 7; ++i){
        __moves[i] = new int[4]; //4 colonnes
    }
}

User::~User(){
    for(int i =0; i < 7; ++i){
		delete[] __moves[i];
    }
    delete[] __moves;
    if(manager_ != NULL){
		
		delete manager_; //Problem if the user has created an auction and quits the game
	}
	if(calendar_ != NULL){
		delete calendar_;
	}
}

void User::cmdHandler() {
	SerializedObject receivedObject = receiveOnSocket(sockfd_);
	SerializedObject *received = &receivedObject;
	
#ifdef __DEBUG
	std::cout<<"En-tête reçu : "<<received->typeOfInfos<<std::endl;
#endif
	if(isAboutManagement(received)){
#ifdef __DEBUG
	std::cout<<"C'est une requête de Management."<<std::endl;
#endif
		handleManagementRequest(received);
	}
	else if (isAboutMatch(received)){
#ifdef __DEBUG
	std::cout<<"C'est une requête de Match."<<std::endl;
#endif
		handleMatchRequest(received);
	}
	else if (isAboutAuctions(received)){
#ifdef __DEBUG
	std::cout<<"C'est une requête d'enchère."<<std::endl;
#endif
		handleAuctionRequest(received);
	}
	else {
		if (manager_!=NULL){
			if (manager_->getLogin()!="admin"){
				calendar_->update();
				DataBase::save(*manager_);
			}
		}
		setDisconnection();
	}
}

void User::handleManagementRequest(SerializedObject *received){
	int targetedBuilding, resultOfUpgrade, size;
	int APGained;
	int i;
	int confirmation;
	std::vector<int> IDList;
    std::vector<std::string> namesList;
    vector<int> infos;
     
	char * position;
	char * answerPosition = _answer.stringData;
	vector<string> playersList;
	int targetedPlayer;
	position = received->stringData;
	int elapsedTime, amount, price;
	switch(received->typeOfInfos){
		//décodage du buffer en fonction de son entête
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
				std::string strName = username;
				bool alreadyConnected = false;
				for(unsigned int i = 0; i < server_->usersList_.size(); ++i){
					alreadyConnected = alreadyConnected or (not strName.compare(server_->usersList_[i]->getUserName())); 
					//compare : 0 if equal, so need to negate that value for bool equivalence
				}
				if(alreadyConnected){
					std::cout<<"ALREADY CONNECTED"<<std::endl;
					confirmation = LOGIN_FAILED;
				}else{
					
					std::cout<<"LOGIN OK"<<std::endl;
					manager_ = new Manager(username);
					userName_=username;
					//userId_=server_->usersList_.size();

					if(!strcmp(username, "admin")){ //return 0 if equal
						state_= ADMIN;
						confirmation = ADMIN_LOGIN;
					}else{
						calendar_ = new Calendar(manager_);
						calendar_->update();
						//DataBase::save(*manager_);
					
						state_=FREE;
						confirmation = NORMAL_LOGIN;
					}
				}
			}
			else {
				std::cout<<"WRONG LOGIN/PASSWORD"<<std::endl;
				confirmation = LOGIN_FAILED;
			}
			//construct _answer
			_answer.typeOfInfos = LOGIN_CONFIRM;

			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			
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
			confirmation = LOGIN_FAILED;
			if ((checkLoginAndPassword(username,password)==-1)||(checkLoginAndPassword(username,password)==1)) { //This Manager login already exists
				std::cout<<"LOGIN ALREADY TAKEN"<<std::endl;
			}
			else {
				std::cout<<"CREATING MANAGER..."<<std::endl;
				addManager(username,password);
				manager_ = new Manager(username);
				userName_=username;
				//userId_=server_->usersList_.size();
				if (userName_ != "admin") DataBase::save(*manager_);
				calendar_ = new Calendar(manager_);

				state_=FREE;
				confirmation = NORMAL_LOGIN;
			}
			//construct _answer
			_answer.typeOfInfos = LOGIN_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			break;

		case GETMANAGERINFOS : {
			//no detail to read
#ifdef __DEBUG
			std::cout<<"Demande des infos du manager reçue sur socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			
			calendar_->update();
			//DataBase::save(*manager_);
			int nbPlayers;
			nbPlayers = manager_->getNumberOfPlayers();
			int money;
			money = manager_->getMoney();
			int nbFans;
			nbFans = manager_->getNumberOfFans();
			int actionPoints;
			actionPoints = manager_->getActionPoints();
			infos.push_back(nbPlayers);
			infos.push_back(money);
			infos.push_back(nbFans);
			infos.push_back(actionPoints);
#ifdef __DEBUG
			std::cout<<"Number of players : "<<manager_->getNumberOfPlayers()<<std::endl;
			std::cout<<"Money : "<<manager_->getMoney()<<std::endl;
			std::cout<<"Number of fans : "<<manager_->getNumberOfFans()<<std::endl;
#endif
			calendar_->update();
			//DataBase::save(*manager_);
			//construct _answer
			_answer.typeOfInfos = MANAGERINFOS;
			writeIntVector(answerPosition, infos);
			sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			break;
		}
		case GETPLAYERSLIST :
			//no detail to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des joueurs reçue sur socket "<<getSockfd()<<std::endl;
#endif
			//handle demand

			calendar_->update();
			//DataBase::save(*manager_);
			playersList = manager_->getPlayersList();
			//construct _answer 
			_answer.typeOfInfos = PLAYERSLIST;
			size = playersList.size();
			
			for (i=0;i<size;i+=2){
				std::cout<<playersList[i]<<" "<<playersList[i+1]<<std::endl;
			}
			memcpy(answerPosition, &size, sizeof(size)); //nb de noms à lire
			answerPosition += sizeof(size);

				//un managedPlayer dans la liste =
				//indice dans la liste déductible par l'ordre en renvoi
				//nom : char[30]
				//prénom : char[30]
				//Attention : on suppose le buffer assez grand
			writeStringVector(answerPosition, playersList);
			calendar_->update();
			//DataBase::save(*manager_);
			sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			
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
			//DataBase::save(*manager_);
			//construct _answer
			_answer.typeOfInfos = PLAYERINFOS;
			//5 attributs int
			//5 états d'entrainements d'attribut int
			//1 int blocked
			//1 int bonus du balais
			//1 int capacity du balais
			//TODO : ajouter la vie
			writeIntVector(answerPosition, infos);
			sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			
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
			//DataBase::save(*manager_);
			//RecrutmentCenter inutilisé
			infos=manager_->getBuildingInformations(targetedBuilding);
			

			calendar_->update();
			DataBase::save(*manager_);
	
			//construct _answer

			_answer.typeOfInfos = BUILDINGINFOS;
			writeIntVector(answerPosition, infos);
			sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
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
			//DataBase::save(*manager_);
			confirmation = false;
			resultOfUpgrade= manager_->startBuildingConstruction(targetedBuilding);
			if (resultOfUpgrade==ALREADYINCONSTRUCTION) std::cout<<"Already in construction !"<<std::endl;
			else if (resultOfUpgrade==NOTENOUGHMONEY) std::cout<<"Not enough money !"<<std::endl; //testé au niveau client
			else if (resultOfUpgrade==NOTENOUGHAP) std::cout<<"Not enough action points !"<<std::endl;
			else {
				std::cout<<"Construction started !"<<std::endl;
				confirmation = true;
			}
			calendar_->update();
			//DataBase::save(*manager_);
			//construct _answer
			_answer.typeOfInfos = UPGRADE_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
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
			//DataBase::save(*manager_);
			confirmation = manager_->trainPlayer(targetedPlayer,capacityToTrain);
			if (!confirmation) std::cout<<"This player is already blocked by a training or the hospital"<<std::endl;
			calendar_->update();
			//DataBase::save(*manager_);

			//construct _answer
			_answer.typeOfInfos = TRAINING_STARTED;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
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
			//DataBase::save(*manager_);
			confirmation = manager_->healPlayer(targetedPlayer);
			if (!confirmation) std::cout<<"This player is already blocked by a training or the hospital or is in full health"<<std::endl;
			calendar_->update();
			//DataBase::save(*manager_);

			//construct _answer
			_answer.typeOfInfos = HEAL_STARTED;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			break;

		case BUY_ACTION_POINTS :
			//reading details
			memcpy(&amount, position, sizeof(amount)); 
#ifdef __DEBUG
			std::cout<<"Demande d'achat de points d'action reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			calendar_->update();
			//DataBase::save(*manager_);
			price = manager_->payForActionPoints(amount);
			calendar_->update();
			//DataBase::save(*manager_);

			//construct _answer
			_answer.typeOfInfos = PRICE_FOR_AP;
			memcpy(answerPosition, &price, sizeof(price));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			break;

		case START_PROMOTION :
			//reading details
#ifdef __DEBUG
			std::cout<<"Demande de début de campagne de promotion sur sockfd "<<getSockfd()<<std::endl;
#endif
			//handle demand
			calendar_->update();
			//DataBase::save(*manager_);
			time_t secondes;
			time(&secondes);
			timeOfStart_ =*localtime(&secondes);
			
			/*confirmation = true;
			
			//construct _answer
			_answer.typeOfInfos = HEAL_STARTED;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour*/
			break;

		case END_PROMOTION :
			//reading details
#ifdef __DEBUG
			std::cout<<"Fin de la campagne de promotion pour le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			calendar_->update();
			//DataBase::save(*manager_);
			elapsedTime = calendar_->getElapsedTime(timeOfStart_);
			APGained = manager_->waitForActionPoints(elapsedTime);

			//DataBase::save(*manager_);

			std::cout<<" ACTION POINTS GAINED : "<<APGained<<endl;
			//construct _answer
			_answer.typeOfInfos = RESULT_PROMOTION;
			memcpy(answerPosition, &APGained, sizeof(APGained));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			break;
			
		case GETMANAGERSLIST : {

			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des managers reçue sur le socket "<<getSockfd()<<std::endl;
#endif

            int counter;
            counter = 0;
            for (unsigned int i=0;i<server_->usersList_.size();++i){
                if((server_->usersList_[i]->state_==FREE) and (server_->usersList_[i] != this)) { //on ne reprend pas celui qui fait la demande
                    ++counter;
                    IDList.push_back(server_->usersList_[i]->getUserID());
                    std::cout << "userId " << server_->usersList_[i]->getUserID() << std::endl;
                    std::cout << "name " << server_->usersList_[i]->getUserName() << std::endl;
                    namesList.push_back(server_->usersList_[i]->getUserName());
                }
            }
            //construct _answer
			_answer.typeOfInfos = MANAGERSLIST;
			memcpy(answerPosition, &counter, sizeof(counter));
            answerPosition += sizeof(counter);
            for(int i = 0; i < counter; ++i){
                int ID = IDList[i];
                char name[30];
                std::string strName = namesList[i];
                strcpy(name, strName.c_str());
                memcpy(answerPosition, &ID, sizeof(ID));
				answerPosition += sizeof(ID);
				memcpy(answerPosition, &name, sizeof(name));
				answerPosition += sizeof(name);
            }
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			break;
		}
	}
}
			
			
void User::handleMatchRequest(SerializedObject *received){
	int confirmation;
	int targetedUser, targetedPlayer;
	 
	char * position;
	char * answerPosition = _answer.stringData;
	position = received->stringData;
	switch(received->typeOfInfos){
		case PROPOSEMATCH : {
			//reading details
			position = received->stringData;

			memcpy(&targetedUser, position, sizeof(targetedUser));

			position += sizeof(targetedUser);
			std::vector<int> playersInTeam = readIntVector(position, 7); //indice des ManagedPlayer à faire jouer
#ifdef __DEBUG
			std::cout<<"Demande de proposition d'un match reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"userID reçu : "<<targetedUser<<std::endl;
#endif

            std::vector<ManagedPlayer> team1;
            for(int i = 0; i < 7; ++i){
                //TODO : éviter des copies ?
				team1.push_back(manager_->getPlayer(playersInTeam[i])); //ajout à la liste
				_teamInMatch.push_back(&manager_->getPlayer(playersInTeam[i]));
			}
            User *invited = NULL;
            for(unsigned int i = 0; i < server_->usersList_.size(); ++i){
                if(server_->usersList_[i]->getUserID() == targetedUser){
                    invited = server_->usersList_[i];
                }
            }
               
            __matchesHandler->proposeForMatch(this, invited, team1, __moves); //matchesHandler handle the _answer
			break;
        }
		case ACCEPTMATCH : {
			//reading details
			bool confirmation; 
			position = received->stringData;
			memcpy(&confirmation, position, sizeof(confirmation));
			position += sizeof(confirmation);
			std::vector<int> playersInTeam; //indice des ManagedPlayer à faire jouer
			if(confirmation){
				playersInTeam = readIntVector(position, 7);
			}
#ifdef __DEBUG
			std::cout<<"Demande d'acceptation d'un match reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"confirmation : "<<confirmation<<std::endl;
#endif

			std::vector<ManagedPlayer> team2;
            for(unsigned int i = 0; i < playersInTeam.size(); ++i){
                //TODO : éviter des copies ?
				team2.push_back(manager_->getPlayer(playersInTeam[i])); //ajout à la liste
				_teamInMatch.push_back(&manager_->getPlayer(playersInTeam[i]));
			}
			__matchesHandler->respondToMatchProposal(this, team2, __moves);

			break;
        }
		case IS_MATCH_WAITING : {
			//no details to read
			//on suppose qu'un seul match sera demandé à la fois
#ifdef __DEBUG
			std::cout<<"Demande si match demandé reçue sur socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			bool confirmation = __matchesHandler->isInvited(this); //TODO : récupérer ID et name de l'invitant
			
			//construct _answer
			_answer.typeOfInfos = MATCH_WAITING;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			break;
		}
		case MAKEMOVES : {
			//reading details
			//soit un joueur se déplace, soit un joueur fait se déplacer une balle, soit ne fait rien : nbmax de mouvements = nbtotal de mouvement = nbjoueurs = 7
			//structure d'un mouvement : int playerID, int diagDest, int lineDest
			
			std::vector<int> receivedMoves = readIntVector(position, 7 * 4);
			for(unsigned int i = 0; i < receivedMoves.size(); i+=4){
				__moves[i/4][0] = receivedMoves[i];
				__moves[i/4][1] = receivedMoves[i+1];
				__moves[i/4][2] = receivedMoves[i+2];
				__moves[i/4][3] = receivedMoves[i+3];
			}
#ifdef __DEBUG
			std::cout<<"Liste de déplacements reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handling demand and answering by the MatchesHandler
            __matchesHandler->recordMoves(this);
			break;
        }
        
        case FORFEIT : {
			__matchesHandler->forfeit(this);
			break;
		}
		case ASKFORDRAW : {
			__matchesHandler->transmitDrawRequest(this);
			break;
		}
		case ANSWERTODRAWPROPOSITION : {
			position = received->stringData;
			memcpy(&confirmation, position, sizeof(confirmation)); 
			__matchesHandler->confirmDraw(this,confirmation);
			break;
		}
		
		case PLAYTRAININGMATCH : {
			//reading details
			position = received->stringData;
			std::vector<int> playersInTeam = readIntVector(position, 7); //indice des ManagedPlayer à faire jouer
#ifdef __DEBUG
			std::cout<<"Demande de début de match d'entrainement reçue sur socket "<<getSockfd()<<std::endl;
#endif
            std::vector<ManagedPlayer> team1;
            for(int i = 0; i < 7; ++i){
                //TODO : éviter des copies ?
				team1.push_back(manager_->getPlayer(playersInTeam[i])); //ajout à la liste
				_teamInMatch.push_back(&manager_->getPlayer(playersInTeam[i]));
			}
               
            __matchesHandler->playTrainingMatch(this, team1, __moves); //matchesHandler handle the _answer
			break;
		}
		
		case GETPOSITIONS :
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des positions sur le terrain reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			_answer.typeOfInfos = POSITIONS;
			__matchesHandler->getScoresAndPositions(this, answerPosition); //appelera sendAnswer puis poursuivra son traitement
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
			_answer.typeOfInfos = PLAYERINFOS;
			__matchesHandler->getPlayerInfos(this, targetedPlayer, answerPosition);
			sendOnSocket(sockfd_, _answer);
			break;
		case CREATE_TOURNAMENT : {
			//reading details
			int nbOfPlayers;
			int startingPrice;
			position = received->stringData;
			memcpy(&nbOfPlayers, position, sizeof(nbOfPlayers));
			position += sizeof(nbOfPlayers);
			memcpy(&startingPrice, position, sizeof(startingPrice));
#ifdef __DEBUG
			std::cout<<"Demande de création d'un tournoi reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"nbOfplayers :  "<<nbOfPlayers<< " startingPrice : " << startingPrice << std::endl;
#endif
			//handling request
			confirmation = __matchesHandler->createTournament(nbOfPlayers, startingPrice);
			
			//answering
			//_answer in confirmation
			_answer.typeOfInfos = NEWTOURNAMENT_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); 
			break;
		}
		case GETTOURNAMENTSLIST : {
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des tournois reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handling request and answering

			_answer.typeOfInfos = TOURNAMENTSLIST;
			confirmation = __matchesHandler->serializeTournaments(answerPosition); //always true ?
			sendOnSocket(sockfd_, _answer); 
			break;
		}
		case JOINTOURNAMENT : {
			//reading details
			int tournamentID;
			position = received->stringData;
			memcpy(&tournamentID, position, sizeof(tournamentID));
#ifdef __DEBUG
			std::cout<<"Demande de participation à un tournoi reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"tournamentID :  "<<tournamentID<< std::endl;
#endif
			//handling request
			_answer.typeOfInfos = JOINTOURNAMENT_CONFIRM;
			confirmation = __matchesHandler->addPlayerToTournament(this, answerPosition); //return -1 if already in list, 0 if not full, 1 if full after addition
						
			break;
		}
		case STARTTOURNAMENTMATCH : {
			//reading details
			position = received->stringData;
			std::vector<int> playersInTeam = readIntVector(position, 7); //indice des ManagedPlayer à faire jouer

			
#ifdef __DEBUG
			std::cout<<"Demande d'acceptation d'un match de tournoi reçue sur socket "<<getSockfd()<<std::endl;
#endif

			std::vector<ManagedPlayer> team;
            for(unsigned int i = 0; i < playersInTeam.size(); ++i){
                //TODO : éviter des copies ?
				team.push_back(manager_->getPlayer(playersInTeam[i])); //ajout à la liste
				_teamInMatch.push_back(&manager_->getPlayer(playersInTeam[i]));
			}
#ifdef __DEBUG
			std::cout<<"On passe le relais à matchesHandler"<<std::endl;
#endif
			try{
			__matchesHandler->respondToTournamentMatch(this, team, __moves);
			}
			catch(const char * msg){
				std::cout<<msg<<std::endl;
			}
			break;
		}
		default :
#ifdef __DEBUG
			std::cout<<"En-tête inconnu : "<<received->typeOfInfos<<std::endl;
#endif
			break;

	}
}
		
void User::handleAuctionRequest(SerializedObject *received){
	int confirmation, size;
	int targetedAuction, targetedPlayer;
	int isFinished;
	int auctionPrice;
	vector<int> infos;
	int currentPrice,auctionTimeLeft;
	ManagedPlayer tmpPlayer;
	vector<string> auctionsList;
	 
	char * position;
	char * answerPosition = _answer.stringData;
	//int resultOfAuction;
	position = received->stringData;
	switch(received->typeOfInfos){
		case CREATEAUCTION : {
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
			//handle demand:
			calendar_->update();
			if (manager_->getActionPoints()>=AP_AUCTION) {
				manager_->setActionPoints(manager_->getActionPoints()-AP_AUCTION);
				//DataBase::save(*manager_);
				tmpPlayer = manager_->getPlayer(targetedPlayer);
				if (tmpPlayer.isBlocked()) confirmation=false;
				else {
					manager_->sellPlayer(targetedPlayer);
					manager_->lockPlayer(tmpPlayer.getFirstName()+" "+tmpPlayer.getLastName());
					server_->createAuction(this,tmpPlayer,startingPrice);
					confirmation=true;
				}
			}
			else confirmation=false;
			_answer.typeOfInfos = AUCTIONCREATION_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			break;
		}
		case JOINAUCTION :
			//reading details
			position = received->stringData;
			memcpy(&targetedAuction, position, sizeof(targetedAuction));
#ifdef __DEBUG
			std::cout<<"Demande de participation à une enchère reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"targetedAuction reçu : "<<targetedAuction<<std::endl;
#endif
			//handle demand:
			calendar_->update();
			if (manager_->getActionPoints()>=AP_ENTER_AUCTION) {
				manager_->setActionPoints(manager_->getActionPoints()-AP_ENTER_AUCTION);
				//DataBase::save(*manager_);
				for (unsigned int i=0;i<server_->auctionsList_.size();++i) {
					if (targetedAuction==server_->getAuctionID(i)) auction_ = server_->auctionsList_[i];
				}
				if (auction_==NULL) confirmation = false;
				else confirmation = true;
				//construct _answer:
			}
			else confirmation=false;
			_answer.typeOfInfos = AUCTIONJOIN_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
			break;



		case GETAUCTIONSLIST :
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des enchères reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand:
			calendar_->update();
			//DataBase::save(*manager_);
			for (unsigned int i=0;i<server_->auctionsList_.size();++i) {
				if (server_->getAuctionTimeLeft(i)>0) {
					auctionsList.push_back(intToString(server_->getAuctionID(i)));
					auctionsList.push_back(server_->getPlayerSoldName(i));
					auctionsList.push_back(intToString(server_->getAuctionTimeLeft(i)));
					auctionsList.push_back(intToString(server_->getAuctionStartingPrice(i)));
				}
				else {
					server_->auctionsList_[i]->endAuction();
				}
			}
			//construct _answer 
			_answer.typeOfInfos = AUCTIONSLIST;
			size = auctionsList.size();
			std::cout<<size<<std::endl;
			memcpy(answerPosition, &size, sizeof(size)); //nb de noms à lire
			answerPosition += sizeof(size);
			for (int i=0;i<int(auctionsList.size()) - 1;i+=4) { //un paquet par joueur
				char auctionID[2*USERNAME_LENGTH], playerName[2*USERNAME_LENGTH], timeLeft[2*USERNAME_LENGTH],price[2*USERNAME_LENGTH];
				string param1 = auctionsList[i];
				string param2 = auctionsList[i+1];
				string param3 = auctionsList[i+2];
				string param4 = auctionsList[i+3];
				strcpy(auctionID, param1.c_str());
				strcpy(playerName, param2.c_str());
				strcpy(timeLeft, param3.c_str());
				strcpy(price, param4.c_str());
				memcpy(answerPosition, &auctionID, sizeof(auctionID));
				answerPosition += sizeof(auctionID);
				memcpy(answerPosition, &playerName, sizeof(playerName));
				answerPosition += sizeof(playerName);
				memcpy(answerPosition, &timeLeft, sizeof(timeLeft));
				answerPosition += sizeof(timeLeft);
				memcpy(answerPosition, &price, sizeof(price));
				answerPosition += sizeof(price);
				
			}

			sendOnSocket(sockfd_, _answer);
			break;

		case GETAUCTIONINFO :

			position = received->stringData;
			memcpy(&targetedAuction, position, sizeof(targetedAuction));
#ifdef __DEBUG
			std::cout<<"Demande de la liste des enchères reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand:
			calendar_->update();
			//DataBase::save(*manager_);
			for (unsigned int i=0;i<server_->auctionsList_.size();++i) {
				if (server_->getAuctionID(i)==targetedAuction) {
					infos= server_->getPlayerSoldInfos(i);
				}
			}
			_answer.typeOfInfos = PLAYERINFOS;
			//5 attributs int
			//5 états d'entrainements d'attribut int
			//1 int blocked
			//1 int bonus du balais
			//1 int capacity du balais
			//TODO : ajouter la vie //done
			writeIntVector(answerPosition, infos);
			sendOnSocket(sockfd_, _answer); //TODO : tester valeur retour
				
			break;


		case BID :
			//no details to read : possible improvement : participation to several auctions at a same time. No.
#ifdef __DEBUG
			std::cout<<"Enchère reçue sur le socket "<<getSockfd()<<std::endl;
#endif

			calendar_->update();
			//DataBase::save(*manager_);
			auction_->bid(this);
			//construct _answer:
			confirmation=true;
			_answer.typeOfInfos = BID_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
			sendOnSocket(sockfd_, _answer);
			break;

		case END_AUCTION_TURN : {
			//no details to read : possible improvement : participation to several auctions at a same time. No.
#ifdef __DEBUG
			std::cout<<"Tour d'enchère fini sur le socket "<<getSockfd()<<std::endl;
#endif
			auction_->endOfTurnAsked();
			calendar_->update();
			//DataBase::save(*manager_);
			isFinished = auction_->isAuctionFinished();
			auctionPrice = auction_->getCurrentPrice();
			int resultOfAuction=0;
			if (isFinished!=0) {
				if (this==auction_->getAuctionCreator()) {
					manager_->unlockPlayer(auction_->getPlayerName());
				}
				if (auction_->getLastBidder()==this) {
					resultOfAuction = -1;
					if (manager_->getMoney()<auctionPrice) resultOfAuction = -2;
					if (auction_->getAuctionCreator()==NULL) auctionWin(auction_,NULL,auction_->getPlayer());
					else auctionWin(auction_,auction_->getManager(),auction_->getPlayer());
				}
				else {
					calendar_->update();
					//DataBase::save(*manager_);
				}
			}
			else {
				resultOfAuction=1;
			}

			if (auction_->getNbOfEndOfTurn()==auction_->getNumberOfBidders()) {
				auction_->resetBidders();
			}
			//construct _answer:
			_answer.typeOfInfos = AUCTION_RESULT;
			memcpy(answerPosition, &resultOfAuction, sizeof(resultOfAuction));
            sendOnSocket(sockfd_, _answer); 
			break;	
		}
		case GET_AUCTION_PRICE : {
			//no details to read : possible improvement : participation to several auctions at a same time. No.
#ifdef __DEBUG
			//std::cout<<"Montant de l'enchère demandé sur le socket "<<getSockfd()<<std::endl;
#endif
			//calendar_->update();
			//DataBase::save(*manager_);
			currentPrice = auction_->getCurrentPrice();

			_answer.typeOfInfos = AUCTION_PRICE;
			memcpy(answerPosition, &currentPrice, sizeof(currentPrice));
            sendOnSocket(sockfd_, _answer); 
			break;	
		}
		case GET_AUCTION_TIME_LEFT : {
			position = received->stringData;
			memcpy(&targetedAuction, position, sizeof(targetedAuction));
			//no details to read : possible improvement : participation to several auctions at a same time. No.
#ifdef __DEBUG
			//std::cout<<"Montant de l'enchère demandé sur le socket "<<getSockfd()<<std::endl;
#endif
			//calendar_->update();
			//DataBase::save(*manager_);
			std::cout<<"here"<<std::endl;
			auctionTimeLeft = server_->getAuctionTimeLeft(targetedAuction);

			std::cout<<"there"<<std::endl;
			_answer.typeOfInfos = AUCTION_TIME_LEFT;
			memcpy(answerPosition, &auctionTimeLeft, sizeof(auctionTimeLeft));
            sendOnSocket(sockfd_, _answer); 
			break;	
		}
	}
}
		

void User::setDisconnection() {
	//ajouter ici les actions à prendre en cas de déconnection
	state_=DISCONNECTING;
}

bool User::isDisconnecting() {return (state_==DISCONNECTING);}
int User::getSockfd() {return sockfd_;}

int User::getUserID() {return userId_;}

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

void User::auctionWin(Auction* auction, Manager* manager, ManagedPlayer player) {
	int amount = auction_->getCurrentPrice();
	if (manager_->getMoney()<amount) {
		
		manager_->pay(manager_->getMoney()/33);

		if (manager==NULL){
			Manager tmpManager = Manager(auction->getManagerLogin());
			tmpManager.addMoney(manager_->getMoney()/33);
			tmpManager.unlockPlayer(player.getFirstName()+" "+player.getLastName());
			DataBase::save(tmpManager);

		}
		else{
			manager->addMoney(manager_->getMoney()/33);
			manager->unlockPlayer(player.getFirstName()+" "+player.getLastName());
			
			
		}
	}
	else {
		manager_->pay(amount);
		manager_->addPlayer(player);
		manager_->unlockPlayer(player.getFirstName()+" "+player.getLastName());
		DataBase::save(*manager_);
		if (manager==NULL){
			Manager tmpManager = Manager(auction->getManagerLogin());
			tmpManager.addMoney(amount);
			tmpManager.unlockPlayer(player.getFirstName()+" "+player.getLastName());
			tmpManager.removePlayer(player);
			//DataBase::save(tmpManager);
			
		}
		else{

			manager->addMoney(amount);
			manager->unlockPlayer(player.getFirstName()+" "+player.getLastName());
			manager->removePlayer(player);
		}
	}
	//DataBase::save(*manager_);
	//DataBase::save(*manager);
}

string User::intToString(int value) { //TODO : dans une classe générique ?
	char buffer[20];
	sprintf(buffer,"%d",value);
	string tmp = "";
	int i = 0;
	while (buffer[i] != '\0') {
		tmp+=buffer[i];
		++i;
	}
	return tmp;
}

void User::handleEndOfMatch(int numTeam, int numWinningTeam){
	int money = manager_->getIncomeFromMatch(numTeam == numWinningTeam, numTeam == 1); //host if team 1
	manager_->addMoney(money);
}

void User::handleEndOfMatch(int numTeam, int numWinningTeam, int tournamentPrice, std::vector<int> lifes){ //tournamentPrice = 0 if not in a tournament
	int money = manager_->getIncomeFromMatch(numTeam == numWinningTeam, numTeam == 1); //host if team 1
	manager_->addMoney(numTeam == numWinningTeam ? money + tournamentPrice : money);
	for(unsigned int i = 0; i < _teamInMatch.size(); ++i){
		_teamInMatch[i]->setLife(lifes[i]);
	}
	_teamInMatch.clear();
}

void User::handleEndOfTrainingMatch(int numTeam, int numWinningTeam, std::vector<int> lifes){
	srand (time(NULL));
	int boostedCapacity;
	for(unsigned int i = 0; i < _teamInMatch.size(); ++i){
		if(numTeam == numWinningTeam){ //caract boostées seulement si est gagnant
			boostedCapacity = rand() % 5; //1 caract alétoire recoit + 1
			_teamInMatch[i]->setCapacity(boostedCapacity, _teamInMatch[i]->getCapacity(boostedCapacity) + 1);
		}
		_teamInMatch[i]->setLife(lifes[i]);
	}
	_teamInMatch.clear();
}

void User::writeIntVector(char * position, std::vector<int> list){
	for(unsigned int i = 0; i < list.size(); ++i){
		memcpy(position, &list[i], sizeof(int));
		position += sizeof(int);
	}
}

std::vector<int> User::readIntVector(char * position, int nbToRead){
	int value;
	std::vector<int> result;
	for(int i = 0; i < nbToRead; ++i){
		memcpy(&value, position, sizeof(int));
		position += sizeof(int);
		result.push_back(value);
	}
	return result;
}

void User::writeStringVector(char * position, std::vector<std::string> list){
	//Attention : on suppose le buffer assez grand
	for (unsigned int i=0;i<list.size() ;++i) {
		char name[USERNAME_LENGTH];
		string stringname = list[i];
		strcpy(name, stringname.c_str());
		memcpy(position, &name, sizeof(name));
		position += sizeof(name);
	}
}


int User::sendInvitation(int IDInvitor, std::string &nameInvitor){
	SerializedObject toSend;
	toSend.typeOfInfos = MATCH_INVITATION; //paquet header
	char * position = toSend.stringData;
	char name[USERNAME_LENGTH];
	strcpy(name, nameInvitor.c_str());
    memcpy(position, &IDInvitor, sizeof(IDInvitor));
    position += sizeof(IDInvitor);
    memcpy(position, &name, sizeof(name));
    return sendOnSocket(sockfd_, toSend);
}

int User::inviteForTournamentMatch(int IDOpponent, std::string nameOpponent){
	_answer.typeOfInfos = MATCH_TOURNAMENT_START;
	char * position = _answer.stringData;
	char name[USERNAME_LENGTH];
	strcpy(name, nameOpponent.c_str());
    memcpy(position, &IDOpponent, sizeof(IDOpponent));
    position += sizeof(IDOpponent);
    memcpy(position, &name, sizeof(name));
    position += sizeof(name);
    return sendOnSocket(sockfd_, _answer);
}

int User::sendEndOfMatch(int code){
	SerializedObject toSend;
	char * position = toSend.stringData;
    if(code == FORFEIT){
        toSend.typeOfInfos = OPPONENTFORFEIT;
    }
    else if(code == ASKFORDRAW){
        toSend.typeOfInfos = OPPONENTASKFORDRAW;
    }
    else{
        toSend.typeOfInfos = DRAW_CONFIRM;
        memcpy(position, &code, sizeof(code)); //confirmation : DRAWACCEPTED or DRAWDENIED
    }
    return sendOnSocket(sockfd_, toSend);
}

int User::sendMatchConfirmation(int answerCode){
	 
    _answer.typeOfInfos = MATCH_CONFIRM;
    char * answerPosition = _answer.stringData;
    int confirmation = answerCode;
    memcpy(answerPosition, &confirmation, sizeof(confirmation));
    return sendOnSocket(sockfd_, _answer);
}

int User::sendAnswer(){
	return sendOnSocket(sockfd_, _answer);
}
