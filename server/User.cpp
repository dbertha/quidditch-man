#include "User.hpp"

#include "MatchesHandler.hpp" 
#include "Auction.hpp"
#include "Server.hpp"



User::User(Server * server, MatchesHandler *matchesHandler, int sockfd, int userID): state_(INIT), server_(server), __matchesHandler(matchesHandler), sockfd_(sockfd), userId_(userID), manager_(NULL), calendar_(NULL), auction_(NULL) {
    __moves = new int*[7]; //7 lignes
    for(int i =0; i < 7; ++i){
        __moves[i] = new int[4]; //4 colonnes
    }
    //TODO : delete correspondant dans le destructeur
}

void User::cmdHandler(SerializedObject *received) {
	SerializedObject answer;
	char * position;
	char * answerPosition = answer.stringData;
	int targetedBuilding;
	int targetedUser;
	int targetedPlayer;
	int resultOfUpgrade;
	int confirmation;
	int size;
	int targetedAuction;
	int isFinished;
	int auctionPrice;
	ManagedPlayer tmpPlayer;
	vector<int> infos;
	vector<string> playersList;
	vector<string> auctionsList;
    std::vector<int> IDList;
    std::vector<std::string> namesList;
	position = received->stringData;
#ifdef __DEBUG
	std::cout<<"En-tête reçu : "<<received->typeOfInfos<<std::endl;
#endif
	switch(received->typeOfInfos){
		//décodage du buffer en fonction de son entête
		//TODO : solution + élégante, découpage en fonction du type de jeu concerné
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
				//userId_=server_->usersList_.size();
				if(!strcmp(username, "admin")){ //return 0 if equal
					state_= ADMIN;
					confirmation = ADMIN_LOGIN;
				}else{
					calendar_ = new Calendar(manager_);
					calendar_->update();
					manager_->save();
					
					state_=FREE;
					confirmation = NORMAL_LOGIN;
				}
			}
			else {
				std::cout<<"WRONG LOGIN/PASSWORD"<<std::endl;
				confirmation = LOGIN_FAILED;
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
				manager_->save();
				calendar_ = new Calendar(manager_);

				state_=FREE;
				confirmation = NORMAL_LOGIN;
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

            std::vector<ManagedPlayer> team1;
            for(int i = 0; i < 7; ++i){
                //TODO : éviter des copies ?
				team1.push_back(manager_->getPlayer(playersInTeam[i])); //ajout à la liste
			}
            User *invited = NULL;
            for(unsigned int i; i < server_->usersList_.size(); ++i){

                if(server_->usersList_[i]->getUserID() == targetedUser){
                    invited = server_->usersList_[i];
                }
            }
               
            __matchesHandler->proposeForMatch(this, invited, team1, __moves); //matchesHandler handle the answer
			break;
        }
		case ACCEPTMATCH : {
			//reading details
			bool confirmation; 
			position = received->stringData;
			memcpy(&confirmation, position, sizeof(confirmation));
			position += sizeof(confirmation);
			std::cout << "size of confirmation : " << sizeof(confirmation) << std::endl;
			std::vector<int> playersInTeam; //indice des ManagedPlayer à faire jouer
			if(confirmation){
				for(int i = 0; i < 7; ++i){
					int value;
					std::cout << "départ : " << position - received->stringData << std::endl;
					memcpy(&value,position, sizeof(value));
					position += sizeof(value);
					playersInTeam.push_back(value); //ajout à la liste
				}
			}
#ifdef __DEBUG
			std::cout<<"Demande d'acceptation d'un match reçue sur socket "<<getSockfd()<<std::endl;
			std::cout<<"confirmation : "<<confirmation<<std::endl;
#endif

			std::vector<ManagedPlayer> team2;
            for(unsigned int i = 0; i < playersInTeam.size(); ++i){
				std::cout << " on récupère les objets correspondants " << playersInTeam[i] <<std::endl;
                //TODO : éviter des copies ?
				team2.push_back(manager_->getPlayer(playersInTeam[i])); //ajout à la liste
			}
#ifdef __DEBUG
			std::cout<<"On passe le relais à matchesHandler"<<std::endl;
#endif
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
			
			//construct answer
			answer.typeOfInfos = MATCH_WAITING;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			break;
		}
		case MAKEMOVES : {
			//reading details
			//soit un joueur se déplace, soit un joueur fait se déplacer une balle, soit ne fait rien : nbmax de mouvements = nbtotal de mouvement = nbjoueurs = 7
			//structure d'un mouvement : int playerID, int diagDest, int lineDest
			
			int diagDest;
			int lineDest;
			int specialAction;
			for(int i = 0; i < 7; ++i){
				memcpy(&targetedPlayer, position, sizeof(targetedPlayer));
				position += sizeof(targetedPlayer);
				memcpy(&specialAction, position, sizeof(specialAction));
				position += sizeof(specialAction);
				memcpy(&diagDest, position, sizeof(diagDest));
				position += sizeof(diagDest);
				memcpy(&lineDest, position, sizeof(lineDest));
				position += sizeof(lineDest);
				__moves[i][0] = targetedPlayer;
				__moves[i][1] = specialAction;
				__moves[i][2] = diagDest;
				__moves[i][3] = lineDest;
			}
#ifdef __DEBUG
			std::cout<<"Liste de déplacements reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"1er mouvement reçu : playerID diagDestination lineDestination "<<__moves[0][0] << " " <<__moves[0][2]<< " " << __moves[0][3]<< std::endl;
			std::cout<<"3eme mouvement reçu : playerID diagDestination lineDestination "<<__moves[2][0] << " " <<__moves[2][2]<< " " << __moves[2][3]<< std::endl;
			std::cout<<"7eme mouvement reçu : playerID diagDestination lineDestination "<<__moves[6][0] << " " <<__moves[6][2]<< " " << __moves[6][3]<< std::endl;
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
			manager_->save();
			tmpPlayer = manager_->getPlayer(targetedPlayer);
			if (tmpPlayer.isBlocked()) confirmation=false;
			else {
				server_->createAuction(this,tmpPlayer,startingPrice);
				confirmation=true;
			}
			answer.typeOfInfos = AUCTIONCREATION_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
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
			manager_->save();
			for (unsigned int i=0;i<server_->auctionsList_.size();++i) {
				if (targetedAuction==server_->getAuctionID(i)) auction_ = server_->auctionsList_[i];
			}
			if (auction_==NULL) confirmation = false;
			else confirmation = true;
			//construct answer:

			answer.typeOfInfos = AUCTIONJOIN_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			break;


		case GETMANAGERSLIST : {

			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des managers reçue sur le socket "<<getSockfd()<<std::endl;
#endif

            int counter;
            counter = 0;
            for (unsigned int i=0;i<server_->usersList_.size();++i){
                if(server_->usersList_[i]->state_==FREE) { //TODO : ne pas reprendre l'user qui fait la demande dans la liste
                    ++counter;
                    IDList.push_back(server_->usersList_[i]->getUserID());
                    std::cout << "userId " << server_->usersList_[i]->getUserID() << std::endl;
                    std::cout << "name " << server_->usersList_[i]->getUserName() << std::endl;
                    namesList.push_back(server_->usersList_[i]->getUserName());
                }
            }
            //construct answer
			answer.typeOfInfos = MANAGERSLIST;
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
            sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
			break;
		}
		case GETAUCTIONSLIST :
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des enchères reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand:
			calendar_->update();
			manager_->save();
			for (unsigned int i=0;i<server_->auctionsList_.size();++i) {
				if (server_->getAuctionTimeLeft(i)>0) {
					auctionsList.push_back(intToString(server_->getAuctionID(i)));
					auctionsList.push_back(server_->getPlayerSoldName(i));
					auctionsList.push_back(intToString(server_->getAuctionTimeLeft(i)));
					auctionsList.push_back(intToString(server_->getAuctionStartingPrice(i)));
				}
			}
			//construct answer 
			answer.typeOfInfos = AUCTIONSLIST;
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

			sendOnSocket(sockfd_, answer);
			break;

		case GETAUCTIONINFO :

			position = received->stringData;
			memcpy(&targetedAuction, position, sizeof(targetedAuction));
#ifdef __DEBUG
			std::cout<<"Demande de la liste des enchères reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand:
			calendar_->update();
			manager_->save();
			for (unsigned int i=0;i<server_->auctionsList_.size();++i) {
				if (server_->getAuctionID(i)==targetedAuction) {
					infos= server_->getPlayerSoldInfos(i);
				}
			}
			answer.typeOfInfos = PLAYERINFOS;
			//5 attributs int
			//5 états d'entrainements d'attribut int
			//1 int blocked
			//1 int bonus du balais
			//1 int capacity du balais
			//TODO : ajouter la vie //done
			std::cout << infos.size() << std::endl;
			for(unsigned int i = 0; i < infos.size(); ++i){
				int value;
				value = infos[i];
				memcpy(answerPosition, &value, sizeof(value));
				answerPosition += sizeof(value);
			}
			sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
				
			break;

		case GETPOSITIONS :
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des positions sur le terrain reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handle demand
			__matchesHandler->getScoresAndPositions(this);
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
			__matchesHandler->getPlayerInfos(this, targetedPlayer);
			//construct answer
			break;
		case BID :
			//no details to read : possible improvement : participation to several auctions at a same time. No.
#ifdef __DEBUG
			std::cout<<"Enchère reçue sur le socket "<<getSockfd()<<std::endl;
#endif

			calendar_->update();
			manager_->save();
			auction_->bid(this);
			//construct answer:
			confirmation=true;
			answer.typeOfInfos = BID_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
			sendOnSocket(sockfd_, answer);
			break;

		case END_AUCTION_TURN : {
			//no details to read : possible improvement : participation to several auctions at a same time. No.
#ifdef __DEBUG
			std::cout<<"Tour d'enchère fini sur le socket "<<getSockfd()<<std::endl;
#endif
			calendar_->update();
			manager_->save();
			isFinished = auction_->isAuctionFinished();
			auctionPrice = auction_->getCurrentPrice();
			int result;
			if (isFinished==1) {
				if (auction_->getLastBidder()==this) {
					result = -1;
					if (manager_->getMoney()<auctionPrice) result = -2;
					auctionWin(auction_->getManager(),auction_->getPlayer());
				}
			}
			else {
				auction_->resetBidders();
				result=1;
			}
			calendar_->update();
			manager_->save();
			//construct answer:
			answer.typeOfInfos = AUCTION_RESULT;
			memcpy(answerPosition, &result, sizeof(result));
            sendOnSocket(sockfd_, answer); 
			answer.typeOfInfos = AUCTION_RESULT;
			memcpy(answerPosition, &auctionPrice, sizeof(auctionPrice));
            sendOnSocket(sockfd_, answer); 
			break;	
		}
		case CREATE_TOURNAMENT : {
			//reading details
			int nbOfPlayers;
			int startingPrice;
			position = received->stringData;
			memcpy(&nbOfPlayers, position, sizeof(nbOfPlayers));
			position += sizeof(nbOfPlayers);
			memcpy(&startingPrice, position, sizeof(startingPrice));
#ifdef __DEBUG
			std::cout<<"Demande de création d'un match reçue sur le socket "<<getSockfd()<<std::endl;
			std::cout<<"nbOfplayers :  "<<nbOfPlayers<< " startingPrice : " << startingPrice << std::endl;
#endif
			//handling request
			//TODO
			
			//answering
			//answer in confirmation
			answer.typeOfInfos = NEWTOURNAMENT_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); 
			break;
		}
		case GETTOURNAMENTSLIST : {
			//no details to read
#ifdef __DEBUG
			std::cout<<"Demande de la liste des tournois reçue sur le socket "<<getSockfd()<<std::endl;
#endif
			//handling request
			//TODO
			//answering
			//TODO
			answer.typeOfInfos = TOURNAMENTSLIST;
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
			//TODO
			//answering
			//answer in confirmation
			answer.typeOfInfos = JOINTOURNAMENT_CONFIRM;
			memcpy(answerPosition, &confirmation, sizeof(confirmation));
            sendOnSocket(sockfd_, answer); 
			break;
		}
		default :
#ifdef __DEBUG
			std::cout<<"En-tête inconnu : "<<received->typeOfInfos<<std::endl;
#endif
			break;

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

void User::auctionWin(Manager* manager, ManagedPlayer player) {
	int amount = auction_->getCurrentPrice();
	if (manager_->getMoney()<amount) {
		manager->addMoney(manager_->getMoney()/33);
		manager_->pay(manager_->getMoney()/33);
	}
	else {
		manager_->pay(amount);
		manager->addMoney(amount);
		manager_->addPlayer(player);
		manager->removePlayer(player);
	}
	manager_->save();
	manager->save();
}

string User::intToString(int value) {
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
