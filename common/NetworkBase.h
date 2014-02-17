/* Module réseau en C, gère la communication basique entre serveur et clients */
#ifndef NETWORKBASE_H
#define NETWORKBASE_H
/* TODO : vérifier si modules inutiles */
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "Defines.h"

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

int sendOnSocket(int socketfd, SerializedObject toSend);

SerializedObject receiveOnSocket(int socketfd);
        
    

//~ typedef struct SerializedObjectList_P {
    //~ SerializedObject *paquet;
    //~ struct SerializedObjectList_P *next;
//~ } SerializedObjectList;
//~ 
//~ void initialize(SerializedObjectList *toInit);

/* TODO : detruire list par "free"

//TODO : si on sait quelle taille peut faire la + grande info à faire circuler d'une fois sur le réseau, on peut éviter la gestion d'une liste.
//mais cette gestion permet de ne pas avoir un gros paquet à transmettre quand l'info est petite */

//~ int sendOverNetwork(int socket_fd, SerializedObjectList *listToSend);
//~ 
//~ int receiveOverNetwork(int socket_fd, SerializedObjectList *listWhereToReceive);
        
    
/*format type de la sérialisation d'un objet C++ en un struct à envoyer sur le réseau :
serialize() {
    SerializedObjectList *listToSend = malloc(sizeof(SerializedObjectList));
    SerializedObjectList *currentObjetOfList = listToSend;
    SerializedObjectList *newObjetInList;
    
    while(encore des infos à ajouter){
        SerializedObject *s = malloc(sizeof(SerializedObject));
        s->typeOfInfos = htons(typeID);
        s->senderId = htons(identifiant);
        s->numInSequence = htons(number);
        s->numericData = htonl(data)
        memcpy(s->stringData, data, SERIALISATION_MAXSIZE);
        currentObjetOfList->paquet = s;
        if(encore des infos à ajouter){
            newObjetInList = malloc(sizeof(SerializedObjectList));
            currentObjetOfList->next = newObjetInList;
            currentObjetOfList = newObjetInList;
            }
    return listToSend;
}
*/

/*
format type de la déserialisation d'un struct reçu par le réseau en un objet :
on suppose tous les structs de la liste comme décrivant un seul objet avec des infos supp
deserialize(const SerializedObjectList *listReceived) {
    parcourt de la liste et extraction des infos séquentiellement
    construction de l'objet attendu à partir des données extraites
    return de l'objet
}
*/

#endif
