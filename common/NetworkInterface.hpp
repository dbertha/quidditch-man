#ifndef NETWORKINTERFACE_HPP
#define NETWORKINTERFACE_HPP

/* Interface réseau en C++ qui fera appel au module C */
#include "NetworkBase.h" //SerializedObject

//pour le test en c :
#include <stdio.h>
#include <stdlib.h>








//Q : méthodes membres statiques d'une classe ?

//appelée dans login() du client :
int sendToServer(int sockfd, char username[30], char password[30]){
    //REM : les param reçu ne sont pas des char[N] (sizeof() = N) mais des char * (sizeof() = taille pointeur)
    //TODO : define size
    SerializedObject serialized;
    char * position;
    serialized.typeOfInfos = LOGIN;
    position = serialized.stringData;
    memcpy(position, username, 30);
    position += 30;
    memcpy(position, password, 30);
    //position += sizeof(password);
    return sendOnSocket(sockfd, serialized);
}

//Serveur lit le SerializedObject et le transmet à commandHandler de User
    
    
    


//TODO : répartir les sendToClient et sendToServer dans les dossiers server/ et client/ ?

//Ces fonctions construisent le struct à passer sur le réseau et l'envoient

//~ int sendToClient(const Client &client, const Building &building);
//~ 
//~ int sendToClient(const Client &client, const Player &player);
//~ 
//~ int sendToClient(const Client &client, const Field &field);
//~ 
//~ //
//~ int sendToServer(const Message &msg);


#endif
