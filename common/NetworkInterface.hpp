#ifndef NETWORKINTERFACE_HPP
#define NETWORKINTERFACE_HPP

#include "Defines.hpp" 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define SERIALISATION_MAXSIZE 1025
#define SUCCESS 1

/* recv(socket, *message, sizeToWrite, 0) return sizeWriten if ok
//send(socket, *message, sizeToRead, 0) return sizeRead if ok */

typedef struct {
    /* on manipule des short pour correspondre aux fonctions et htons/ntohs
    on suppose que les types de bases sont de taille identique sur chaque machine */
    short typeOfInfos;
    char stringData[SERIALISATION_MAXSIZE];
} SerializedObject;

SerializedObject receiveOnSocket(int);

int sendOnSocket(int socketfd, SerializedObject toSend);

//appelée dans login() du client :
int sendLoginToServer(int, char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
int sendNewManagerToServer(int, char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
int askForManagerInfos(int);
int askForBuildingInfos(int, int);
int askForBuildingUpgrade(int, int);
int askForPlayersList(int);
int askForPlayerInfos(int, int);
int proposeMatchTo(int, int, std::vector<int> playersInTeam);
int answerMatchProposal(int, bool, std::vector<int> playersInTeam);
int getManagersList(int);

//réception des données du serveur :
bool getConfirmation(int);
int receiveMatchConfirmation(int);
void receiveManagersIDandNames(int, std::vector<int> * IDList, std::vector<std::string> * namesList);
void receiveManagerInfos(int, int *, int *, int *, int *);
std::vector<std::string> receivePlayersList(int);
    //pour chaque ManagedPlayer, on reçoit un string firstname et un lastname, de façon ordonnée (indice 0 à 1 : playerID = 0, etc)
std::vector<int> receivePlayerInfo(int);
std::vector<int> receiveBuildingInfos(int);

#endif
