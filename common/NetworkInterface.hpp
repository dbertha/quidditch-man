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




//Serveur lit le SerializedObject et le transmet à commandHandler de User
    



#endif
