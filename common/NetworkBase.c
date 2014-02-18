/* Module réseau en C, gère la communication basique entre serveur et clients */

/* TODO : vérifier si modules inutiles 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>*/
#include <stdio.h>
#include <stdlib.h>

#include "Defines.hpp"
#include "NetworkBase.h"

/* recv(socket, *message, sizeToWrite, 0) return sizeWriten if ok
//send(socket, *message, sizeToRead, 0) return sizeRead if ok */


SerializedObject receiveOnSocket(int socketfd){
    SerializedObject received;
    
    int result;
    result = recv(socketfd, &received, sizeof(SerializedObject), 0);
    //Attention, on ne renvoie pas la valeur de retour
    received.typeOfInfos = ntohs(received.typeOfInfos);
    if(result != sizeof(SerializedObject)){
        printf("%s\n", "receiveOnSocket : Erreur dans l'envoi des données");
    }
    return received;
}

int sendOnSocket(int socketfd, SerializedObject toSend){
    int result;
    toSend.typeOfInfos = htons(toSend.typeOfInfos);
    result =  send(socketfd, &toSend, sizeof(SerializedObject), 0);
    if(result != sizeof(SerializedObject)){
        printf("%s\n", "SendOnSocket : Erreur dans l'envoi des données"); //TODO : sur le flux err
    }
    return result;
}

//~ void initialize(SerializedObjectList *toInit){
    //~ toInit->paquet = NULL;
    //~ toInit->next = NULL;
//~ }

/* TODO : detruire list par "free"

//TODO : si on sait quelle taille peut faire la + grande info à faire circuler d'une fois sur le réseau, on peut éviter la gestion d'une liste.
//mais cette gestion permet de ne pas avoir un gros paquet à transmettre quand l'info est petite */

//~ int sendOverNetwork(int socket_fd, SerializedObjectList *listToSend){
    //~ /*envoie la séquence de paquet formée par la liste listToSend
    //~ //TODO : conversion des valeurs avec htonl/ntohl et htons/ntohs */
    //~ while(listToSend != NULL){
        //~ if(send(socket_fd, listToSend->paquet, sizeof(SerializedObject), 0) != sizeof(SerializedObject)){
            //~ return ERROR;
        //~ }
        //~ listToSend = listToSend->next;
    //~ }
    //~ return SUCESS;
//~ }

//~ int receiveOverNetwork(int socket_fd, SerializedObjectList *listWhereToReceive){
    //~ /* on suppose la liste vide, cette fonction va la remplir */
    //~ SerializedObjectList *currentPosition = listWhereToReceive; /* on commence à la tête de liste*/
    //~ SerializedObjectList *newPosition;
    //~ int stillPaquetsRemaining = 1;
    //~ while(stillPaquetsRemaining){
        //~ if(recv(socket_fd, currentPosition->paquet, sizeof(SerializedObject), 0) != sizeof(SerializedObject)){
            //~ return ERROR;
        //~ }
        //~ /* TODO : conversion des valeurs avec htonl/ntohl et htons/ntohs */
        //~ stillPaquetsRemaining = currentPosition->paquet->nbOfFollowing; /* 0 si pas de suivants */
        //~ /* TODO : test que nbOfFollowing baisse bien de 1 à chaque fois */
        //~ if(stillPaquetsRemaining){
            //~ newPosition = malloc(sizeof(SerializedObjectList));
            //~ initialize(newPosition);
            //~ currentPosition->next = newPosition;
            //~ currentPosition = newPosition;
        //~ }
    //~ }
    //~ return SUCESS;
//~ }


