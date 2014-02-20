#ifndef NETWORKINTERFACE_HPP
#define NETWORKINTERFACE_HPP

/* Interface réseau en C++ qui fera appel au module C */
#include "NetworkBase.h" //SerializedObject
#include "Defines.hpp" 

//pour le test en c :
#include <stdio.h>
#include <stdlib.h>

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

int answerMatchProposal(int sockfd, int askerID, bool confirmation, std::vector<int> playersInTeam){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = ACCEPTMATCH;
    memcpy(position, &askerID, sizeof(askerID));
    position += sizeof(askerID);
    memcpy(position, &confirmation, sizeof(confirmation));
    position += sizeof(confirmation);
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

int sendMoves(int sockfd, int moves[][3]){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = MAKEMOVES;
    int targetedPlayer;
    int diagDest;
    int lineDest;
    for(int i = 0; i < 7; ++i){
        
        targetedPlayer = moves[i][0];
        diagDest = moves[i][1];
        lineDest = moves[i][2];
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

//réception des donnéés du serveur :

bool getConfirmation(int sockfd){ //valable pour LOGIN_CONFIRM, UPGRADE_CONFIRM, TRAINING_STARTED
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
        for(int i = 0; i < 13; ++i){
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




//Serveur lit le SerializedObject et le transmet à commandHandler de User
    



#endif
