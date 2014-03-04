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

SerializedObject receiveOnSocket(int socketfd);

int sendOnSocket(int socketfd, SerializedObject toSend);

//appelée dans login() du client :
int sendLoginToServer(int sockfd, char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);

int sendNewManagerToServer(int sockfd, char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);

int proposeMatchTo(int sockfd, int userID, std::vector<int> playersInTeam);

int answerMatchProposal(int sockfd, bool confirmation, std::vector<int> playersInTeam);

int getManagersList(int sockfd);

//réception des données du serveur :
bool getConfirmation(int sockfd);

int receiveMatchConfirmation(int sockfd);

void receiveManagersIDandNames(int sockfd, std::vector<int> * IDList, std::vector<std::string> * namesList);
#endif
