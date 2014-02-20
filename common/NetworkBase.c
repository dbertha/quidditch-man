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
    }else{
#ifdef __DEBUG
    printf("%s\n", "SendOnSocket : Paquet envoyé");
#endif
    }
    return result;
}




