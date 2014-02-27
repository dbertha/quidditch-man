#ifndef NETWORKINTERFACE_HPP
#define NETWORKINTERFACE_HPP

/* Interface réseau en C++ qui fera appel au module C */
#include "NetworkBase.h" //SerializedObject
#include "Defines.hpp" 
#include "HexagonalField.hpp"

//pour le test en c :
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <vector>
#include <iostream>



//Q : méthodes membres statiques d'une classe ?
//TODO : renommage + adapté, "ask serveur" != "ask client"

//TODO : récupérer les méthodes dans la classe client, paramètre socketfd = attribut de la classe
//TODO : forte ressemblance : plus de réutilisation de code : surcharge

//appelée dans login() du client :
int sendLoginToServer(int sockfd, char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]){
    //REM : les param reçu ne sont pas des char[N] (sizeof() = N) mais des char * (sizeof() = taille pointeur)
    //TODO : define size
    SerializedObject serialized;
    char * position;
    serialized.typeOfInfos = LOGIN;
    position = serialized.stringData;
    memcpy(position, username, USERNAME_LENGTH);
    position += USERNAME_LENGTH;
    memcpy(position, password, PASSWORD_LENGTH);
    //position += sizeof(password);
    return sendOnSocket(sockfd, serialized);
}

int sendNewManagerToServer(int sockfd, char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]){
    //REM : les param reçu ne sont pas des char[N] (sizeof() = N) mais des char * (sizeof() = taille pointeur)
    //TODO : define size
    SerializedObject serialized;
    char * position;
    serialized.typeOfInfos = CREATE_MANAGER;
    position = serialized.stringData;
    memcpy(position, username, USERNAME_LENGTH);
    position += USERNAME_LENGTH;
    memcpy(position, password, PASSWORD_LENGTH);
    //position += sizeof(password);
    return sendOnSocket(sockfd, serialized);
}

int askForManagerInfos(int sockfd){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETMANAGERINFOS;
    return sendOnSocket(sockfd, serialized);
}

int askForBuildingInfos(int sockfd, int buildingID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETBUILDINGINFOS;
    memcpy(position, &buildingID, sizeof(buildingID));
    return sendOnSocket(sockfd, serialized);
}

int askForBuildingUpgrade(int sockfd, int buildingID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = UPGRADE_BUILDING;
    memcpy(position, &buildingID, sizeof(buildingID));
    return sendOnSocket(sockfd, serialized);
}

int askForPlayersList(int sockfd){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETPLAYERSLIST;
    return sendOnSocket(sockfd, serialized);
}

int askForPlayerInfos(int sockfd, int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETPLAYERINFOS;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd, serialized);
}


int proposeMatchTo(int sockfd, int userID, std::vector<int> playersInTeam){
    SerializedObject serialized;
    char * position = serialized.stringData;
    int value;
    serialized.typeOfInfos = PROPOSEMATCH;
    memcpy(position, &userID, sizeof(userID));
    position += sizeof(userID);
    for(unsigned int i = 0; i < playersInTeam.size();++i){ //joueurs choisi pour joueur le match
        value = playersInTeam[i];
        memcpy(position, &value, sizeof(value));
        position += sizeof(value);
    }
    
    return sendOnSocket(sockfd, serialized);
}

int answerMatchProposal(int sockfd, bool confirmation, std::vector<int> playersInTeam){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = ACCEPTMATCH;
    memcpy(position, &confirmation, sizeof(confirmation));
    position += sizeof(confirmation);
    int value;
    for(unsigned int i = 0; i < playersInTeam.size();++i){ //joueurs choisi pour joueur le match
        value = playersInTeam[i];
        memcpy(position, &value, sizeof(value));
        position += sizeof(value);
    }
    return sendOnSocket(sockfd, serialized);
}


int isMatchWaiting(int sockfd){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = IS_MATCH_WAITING;
    return sendOnSocket(sockfd, serialized);
}

int sellPlayer(int sockfd, int playerID, int startingPrice){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = CREATEAUCTION;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &startingPrice, sizeof(startingPrice));
    return sendOnSocket(sockfd, serialized);
}

int joinAuction(int sockfd, int auctionID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = JOINAUCTION;
    memcpy(position, &auctionID, sizeof(auctionID));
    return sendOnSocket(sockfd, serialized);
}

int askForAuctionInfos(int sockfd, int auctionID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETAUCTIONINFO;
    memcpy(position, &auctionID, sizeof(auctionID));
    return sendOnSocket(sockfd, serialized);
}


int getManagersList(int sockfd){
    SerializedObject serialized;
    serialized.typeOfInfos = GETMANAGERSLIST;
    return sendOnSocket(sockfd, serialized);
}

int getAuctionsList(int sockfd){
    SerializedObject serialized;
    serialized.typeOfInfos = GETAUCTIONSLIST;
    return sendOnSocket(sockfd, serialized);
}

int getAllPositions(int sockfd){
    SerializedObject serialized;
    serialized.typeOfInfos = GETPOSITIONS;
    return sendOnSocket(sockfd, serialized);
}

int selectPlayer(int sockfd, int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = SELECTPLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd, serialized);
}

int trainPlayer(int sockfd, int playerID, int capacity){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = TRAIN_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &capacity, sizeof(capacity));
    return sendOnSocket(sockfd, serialized);
}

int healPlayer(int sockfd, int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = HEAL_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd, serialized);
}

int sendMoves(int sockfd, int moves[][4]){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = MAKEMOVES;
    int targetedPlayer;
    int specialAction;
    int diagDest;
    int lineDest;
    for(int i = 0; i < 7; ++i){
        
        targetedPlayer = moves[i][0];
        specialAction = moves[i][1];
        diagDest = moves[i][2];
        lineDest = moves[i][3];
        printf("%s : %d %d %d\n", "playerID diagDest lineDest", targetedPlayer, diagDest, lineDest);
        memcpy(position, &targetedPlayer,sizeof(targetedPlayer)); 
        position += sizeof(targetedPlayer);
        memcpy(position, &diagDest, sizeof(diagDest));
        position += sizeof(diagDest); 
        memcpy(position, &lineDest, sizeof(lineDest));
        position += sizeof(lineDest); 
    }
    return sendOnSocket(sockfd, serialized);
}

int getCurrentPrice(int sockfd){
    SerializedObject serialized;
    serialized.typeOfInfos = GETCURRENTPRICE;
    return sendOnSocket(sockfd, serialized);
}

int bid(int sockfd){
    SerializedObject serialized;
    serialized.typeOfInfos = BID;
    return sendOnSocket(sockfd, serialized);
}

int checkAuction(int sockfd){
    SerializedObject serialized;
    serialized.typeOfInfos = END_AUCTION_TURN;
    return sendOnSocket(sockfd, serialized);
}

//réception des donnéés du serveur :

bool getConfirmation(int sockfd){ //valable pour LOGIN_CONFIRM, UPGRADE_CONFIRM, TRAINING_STARTED, HEAL_STARTED, AUCTIONCREATION_CONFIRM
    bool confirmation;
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    //TODO : vérifier qu'il s'agit bien d'un message de confirmation
    memcpy(&confirmation,position, sizeof(confirmation));
    return confirmation;
}

void receiveManagerInfos(int sockfd, int *nbPlayers, int * money, int * nbFans){
    //écrit les données sur les objets pointés
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    if(received.typeOfInfos == MANAGERINFOS){
        memcpy(nbPlayers,position, sizeof(int));
        position += sizeof(int);
        memcpy(money,position, sizeof(int));
        position += sizeof(int);
        memcpy(nbFans,position, sizeof(int));
        position += sizeof(int);
    }
}

std::vector<std::string> receivePlayersList(int sockfd){
    //pour chaque ManagedPlayer, on reçoit un string firstname et un lastname, de façon ordonnée (indice 0 à 1 : playerID = 0, etc)
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    std::vector<std::string> playersListNames;
    if(received.typeOfInfos == PLAYERSLIST){ //on suppose toujours vrai
        int nbNames;
        memcpy(&nbNames,position, sizeof(nbNames));
        position += sizeof(nbNames);
        std::cout << nbNames << std::endl;
        for(int i = 0; i < nbNames; ++i){
            char name[USERNAME_LENGTH];
            std::string strName;
            memcpy(&name,position, sizeof(name));
            position += sizeof(name);
            strName = name; //conversion
            playersListNames.push_back(strName); //ajout à la liste
        }
    }
    return playersListNames;
}
    
std::vector<int> receivePlayerInfo(int sockfd){
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    std::vector<int> playerInfos;
    if(received.typeOfInfos == PLAYERINFOS){ //on suppose toujours vrai
            //5 attributs int
			//5 états d'entrainements d'attribut int
			//1 int blocked
			//1 int bonus du balais
			//1 int capacity du balais
            //1 int life
            //1 int estimated value of player
        for(int i = 0; i < 15; ++i){
            int value;
            memcpy(&value,position, sizeof(value));
            position += sizeof(value);
            playerInfos.push_back(value); //ajout à la liste
        }
    }
    return playerInfos;
}
        

std::vector<int> receiveBuildingInfos(int sockfd){
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    std::vector<int> buildingInfos;
    if(received.typeOfInfos == BUILDINGINFOS){ //on suppose toujours vrai
            //int : level
            //int : pricefornextlevel
            //int : specialAttribut
            //int : enTravaux
        for(int i = 0; i < 4; ++i){
            int value;
            memcpy(&value,position, sizeof(value));
            position += sizeof(value);
            buildingInfos.push_back(value); //ajout à la liste
        }
    }
    return buildingInfos;
}

std::vector<std::string> receiveAuctionsList(int sockfd){
    //pour chaque ManagedPlayer, on reçoit un string firstname et un lastname, de façon ordonnée (indice 0 à 1 : playerID = 0, etc)
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    std::vector<std::string> auctionsList;
    if(received.typeOfInfos == AUCTIONSLIST){ //on suppose toujours vrai
        int nbInfos;
        memcpy(&nbInfos,position, sizeof(nbInfos));
        position += sizeof(nbInfos);
        std::cout << nbInfos << std::endl;
        for(int i = 0; i < nbInfos; ++i){
            char info[2*USERNAME_LENGTH];
            std::string strInfos;
            memcpy(&info,position, sizeof(info));
            position += sizeof(info);
            strInfos = info; //conversion
            auctionsList.push_back(strInfos); //ajout à la liste
        }
    }
    return auctionsList;
}


int receiveAuctionResult(int sockfd) {
    int result;
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    //TODO : vérifier qu'il s'agit bien d'un message de confirmation
    memcpy(&result,position, sizeof(result));
    return result;
}

std::vector<AxialCoordinates> receiveScoresAndPositions(int sockfd, int * winner, int * scoreTeam1, int * scoreTeam2){
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    std::vector<AxialCoordinates> orderedPositions;
    int diag;
    int line;
    memcpy(winner, position, sizeof(int));
    position += sizeof(int);
    memcpy(scoreTeam1, position, sizeof(int));
    position += sizeof(int);
    memcpy(scoreTeam2, position, sizeof(int));
    position += sizeof(int);
    for(unsigned int i = 0; i < 18; ++i){ //positions des 18 objets
        memcpy(&diag, position, sizeof(diag));
        position += sizeof(diag);
        memcpy(&line, position, sizeof(line));
        position += sizeof(line);
        orderedPositions.push_back(AxialCoordinates(diag, line));
    }
    return orderedPositions;
}

//~ std::vector<AxialCoordinates> receiveScoresAndPositions(int sockfd, int * winner, int * scoreTeam1, int * scoreTeam2){
    //~ SerializedObject received = receiveOnSocket(sockfd);
    //~ char * position = received.stringData;
    //~ std::vector<AxialCoordinates> orderedPositions;
    //~ int diag;
    //~ int line;
    //~ memcpy(winner, position, sizeof(int));
    //~ position += sizeof(int);
    //~ memcpy(scoreTeam1, position, sizeof(int));
    //~ position += sizeof(int);
    //~ memcpy(scoreTeam2, position, sizeof(int));
    //~ position += sizeof(int);
    //~ for(unsigned int i = 0; i < 18; ++i){ //positions des 18 objets
        //~ memcpy(&diag, position, sizeof(diag));
        //~ position += sizeof(diag);
        //~ memcpy(&line, position, sizeof(line));
        //~ position += sizeof(line);
        //~ orderedPositions.push_back(AxialCoordinates(diag, line));
    //~ }
    //~ return orderedPositions;
//~ }


typedef struct { //pas besoin de la classe complète
    int attributes[5];
    int hasQuaffle;
} playerAttr;


playerAttr receiveSelectedPlayerInfos(int sockfd){
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    playerAttr thePlayer;
    int attribute;
    for(int i = 0; i < 5; ++i){
	memcpy(&attribute, position, sizeof(attribute));
	position += sizeof(attribute);
	thePlayer.attributes[i] = attribute;
    }
    memcpy(&attribute, position, sizeof(attribute)); //hasQuaffle
    position += sizeof(attribute);
    thePlayer.hasQuaffle = attribute;
    return thePlayer;
}

int receiveMatchConfirmation(int sockfd){
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    int confirmation;
    memcpy(&confirmation, position, sizeof(confirmation));
    return confirmation;
}

void receiveManagersIDandNames(int sockfd, std::vector<int> * IDList, std::vector<std::string> * namesList){
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    int ID;
    char name[USERNAME_LENGTH];
    int nbOfAvailableManagers;
    memcpy(&nbOfAvailableManagers, position, sizeof(nbOfAvailableManagers));
    position += sizeof(nbOfAvailableManagers);
    for(int i = 0; i < nbOfAvailableManagers; ++i){
        memcpy(&ID, position, sizeof(ID));
        position += sizeof(ID);
        IDList->push_back(ID);
        memcpy(&name, position, sizeof(name));
        position += sizeof(name);
        std::string strName = name;
        namesList->push_back(strName);
    }
}
    



#endif
