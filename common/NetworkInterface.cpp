#include "NetworkInterface.hpp"
/* recv(socket, *message, sizeToWrite, 0) return sizeWriten if ok
//send(socket, *message, sizeToRead, 0) return sizeRead if ok */

//~ SerializedObject receiveOnSocket(int socketfd){
    //~ SerializedObject received;
//~ 
    //~ int result;
    //~ result = recv(socketfd, &received, sizeof(SerializedObject), 0);
    //~ //Attention, on ne renvoie pas la valeur de retour
    //~ received.typeOfInfos = ntohs(received.typeOfInfos);
    //~ if(result != sizeof(SerializedObject))
        //~ std::cout<<"receiveOnSocket : Erreur dans la reception des données"<<std::endl;
    //~ return received;
//~ }

//~ int sendOnSocket(int socketfd, SerializedObject toSend){
    //~ int result;
    //~ toSend.typeOfInfos = htons(toSend.typeOfInfos);
    //~ result =  send(socketfd, &toSend, sizeof(SerializedObject), 0);
    //~ if(result != sizeof(SerializedObject))
        //~ std::cout<<"sendOnSocket : Erreur dans l'envoi des données"<<std::endl; //TODO : sur le flux err
    //~ else{
//~ #ifdef __DEBUG
        //~ std::cout<<"SendOnSocket : Paquet envoyé"<<std::endl;
//~ #endif
    //~ }
    //~ return result;
//~ }

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
int trainPlayer(int sockfd_, int playerID, int capacity){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = TRAIN_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &capacity, sizeof(capacity));
    return sendOnSocket(sockfd_, serialized);
}
int healPlayer(int sockfd_, int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = HEAL_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, serialized);
}
int sellPlayer(int sockfd_, int playerID, int startingPrice){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = CREATEAUCTION;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &startingPrice, sizeof(startingPrice));
    return sendOnSocket(sockfd_, serialized);
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
int askForTournamentList(int sockfd){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETTOURNAMENTSLIST;
    return sendOnSocket(sockfd, serialized);
}
int askToJoinTournament(int sockfd, int tournamentID){
    SerializedObject serialized;
    serialized.typeOfInfos = JOINTOURNAMENT;
    char * position = serialized.stringData;
    memcpy(position, &tournamentID, sizeof(tournamentID));
    return sendOnSocket(sockfd, serialized);
}
int sendTournamentCreation(int sockfd, int nbOfPlayers, int startingPrice){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = CREATE_TOURNAMENT;
    memcpy(position, &nbOfPlayers, sizeof(nbOfPlayers));
    position += sizeof(nbOfPlayers);
    memcpy(position, &startingPrice, sizeof(startingPrice));
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
void receiveManagerInfos(int sockfd, int *nbPlayers, int * money, int * nbFans, int * actionPoints){
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
        memcpy(actionPoints,position, sizeof(int));
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
            //1 int life
            //1 int estimated value of player
        for(int i = 0; i < 15; ++i){
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
std::vector<int> getTournamentList(int sockfd_) {
    //nbOfTournaments then __startingNbOfPlayers, __currentNbOfPlayers, __startingPrice for each
    int nbOfTournaments, startingNbOfPlayers, currentNbOfPlayers, startingPrice;
    std::vector<int> tournamentsList;
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    memcpy(&nbOfTournaments, position, sizeof(nbOfTournaments));
    position += sizeof(nbOfTournaments);
    //std::cout << "Nb of tournaments : " << nbOfTournaments << std::endl;
    for(int i = 0; i < nbOfTournaments; ++i){
        memcpy(&startingNbOfPlayers, position, sizeof(startingNbOfPlayers));
        position += sizeof(startingNbOfPlayers);
        memcpy(&currentNbOfPlayers, position, sizeof(currentNbOfPlayers));
        position += sizeof(currentNbOfPlayers);
        memcpy(&startingPrice, position, sizeof(startingPrice));
        position += sizeof(startingPrice);
        tournamentsList.push_back(startingNbOfPlayers);
        tournamentsList.push_back(currentNbOfPlayers);
        tournamentsList.push_back(startingPrice);
    }
    return tournamentsList;

}
