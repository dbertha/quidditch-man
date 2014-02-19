#ifndef NETWORKINTERFACE_HPP
#define NETWORKINTERFACE_HPP

/* Interface réseau en C++ qui fera appel au module C */
#include "NetworkBase.h" //SerializedObject
#include "Defines.hpp" 

//pour le test en c :
#include <stdio.h>
#include <stdlib.h>




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

int proposeMatchTo(int sockfd, int userID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = PROPOSEMATCH;
    memcpy(position, &userID, sizeof(userID));
    return sendOnSocket(sockfd, serialized);
}

int answerMatchProposal(int sockfd, int askerID, int confirmation){
    //TODO : when in c++ code, use bool for confirmation
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = ACCEPTMATCH;
    memcpy(position, &askerID, sizeof(askerID));
    position += sizeof(askerID);
    memcpy(position, &confirmation, sizeof(confirmation));
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






    





//Serveur lit le SerializedObject et le transmet à commandHandler de User
    



#endif