#include "NetworkInterface.hpp"
/* recv(socket, *message, sizeToWrite, 0) return sizeWriten if ok
//send(socket, *message, sizeToRead, 0) return sizeRead if ok */

SerializedObject receiveOnSocket(int socketfd){
    SerializedObject received;

    int result;
    result = recv(socketfd, &received, sizeof(SerializedObject), 0);
    //Attention, on ne renvoie pas la valeur de retour
    received.typeOfInfos = ntohs(received.typeOfInfos);
    if(result != sizeof(SerializedObject))
        std::cout<<"receiveOnSocket : Erreur dans la reception des données"<<std::endl;
    return received;
}

int sendOnSocket(int socketfd, SerializedObject toSend){
    int result;
    toSend.typeOfInfos = htons(toSend.typeOfInfos);
    result =  send(socketfd, &toSend, sizeof(SerializedObject), 0);
    if(result != sizeof(SerializedObject))
        std::cout<<"sendOnSocket : Erreur dans l'envoi des données"<<std::endl; //TODO : sur le flux err
    else{
#ifdef __DEBUG
        std::cout<<"SendOnSocket : Paquet envoyé"<<std::endl;
#endif
    }
    return result;
}

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

int getManagersList(int sockfd){
    SerializedObject serialized;
    serialized.typeOfInfos = GETMANAGERSLIST;
    return sendOnSocket(sockfd, serialized);
}

//réception des données du serveur :

bool getConfirmation(int sockfd){
//valable pour LOGIN_CONFIRM, UPGRADE_CONFIRM, TRAINING_STARTED, HEAL_STARTED, AUCTIONCREATION_CONFIRM
    bool confirmation;
    SerializedObject received = receiveOnSocket(sockfd);
    char * position = received.stringData;
    //TODO : vérifier qu'il s'agit bien d'un message de confirmation
    memcpy(&confirmation,position, sizeof(confirmation));
    return confirmation;
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
