#include "Client.hpp"
// Documentation : voir Readme.txt

using namespace std; 


Client::Client(int sockfd): sockfd_(sockfd) {}


int Client::askForTournamentList(){
    _serialized.typeOfInfos = GETTOURNAMENTSLIST;
    return sendOnSocket(sockfd_, _serialized);
}

std::vector<int> Client::getTournamentList(){
    //nbOfTournaments then __startingNbOfPlayers, __currentNbOfPlayers, __startingPrice for each
    int nbOfTournaments, startingNbOfPlayers, currentNbOfPlayers, startingPrice;
    askForTournamentList(); //demande au serveur
    std::vector<int> tournamentsList;
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    memcpy(&nbOfTournaments, position, sizeof(nbOfTournaments));
    position += sizeof(nbOfTournaments);
    std::vector<int> infos = receiveIntList(position, nbOfTournaments * 3); //3 entiers pour chaque tournoi
    for(int i = 0; i < nbOfTournaments * 3; i +=3){
        //~ memcpy(&startingNbOfPlayers, position, sizeof(startingNbOfPlayers));
        //~ position += sizeof(startingNbOfPlayers);
        //~ memcpy(&currentNbOfPlayers, position, sizeof(currentNbOfPlayers));
        //~ position += sizeof(currentNbOfPlayers);
        //~ memcpy(&startingPrice, position, sizeof(startingPrice));
        //~ position += sizeof(startingPrice);
        tournamentsList.push_back(infos[i]);
        tournamentsList.push_back(infos[i+1]);
        tournamentsList.push_back(infos[i+2]);
    }
    return tournamentsList;
        
}





int Client::sendAnswerToDrawProposition(int code){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = ANSWERTODRAWPROPOSITION;
    memcpy(position, &code, sizeof(code));
    return sendOnSocket(sockfd_, _serialized);
}

int Client::sendForfeit(){
    _serialized.typeOfInfos = FORFEIT;
    return sendOnSocket(sockfd_, _serialized);
}

int Client::sendDrawRequest(){
    _serialized.typeOfInfos = ASKFORDRAW;
    return sendOnSocket(sockfd_, _serialized);
}

int Client::sendTournamentCreation(int nbOfPlayers, int startingPrice){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = CREATE_TOURNAMENT;
    memcpy(position, &nbOfPlayers, sizeof(nbOfPlayers));
    position += sizeof(nbOfPlayers);
    memcpy(position, &startingPrice, sizeof(startingPrice));
    return sendOnSocket(sockfd_, _serialized);
}



int Client::receiveNumOfTeam(){
    return receiveMatchConfirmation();
}


////////Network\\\\\\\\

//login\\

int Client::sendLoginToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH], bool newUser){
    //REM : les param reçu ne sont pas des char[N] (sizeof() = N) mais des char * (sizeof() = taille pointeur)
    char * position;
    _serialized.typeOfInfos = LOGIN;
    if(newUser) _serialized.typeOfInfos =CREATE_MANAGER;
    position = _serialized.stringData;
    memcpy(position, username, USERNAME_LENGTH);
    position += USERNAME_LENGTH;
    memcpy(position, password, PASSWORD_LENGTH);
    return sendOnSocket(sockfd_, _serialized);
}

int Client::sendNewManagerToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]){
    return sendLoginToServer(username, password, true);
}

int Client::getConfirmation(){ //valable pour LOGIN_CONFIRM, UPGRADE_CONFIRM, TRAINING_STARTED, HEAL_STARTED, AUCTIONCREATION_CONFIRM
    int confirmation;
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    //on suppose qu'il s'agit bien d'un message de confirmation
    memcpy(&confirmation,position, sizeof(confirmation));
    return confirmation;
}

//management\\

int Client::askForManagerInfos(){
    _serialized.typeOfInfos = GETMANAGERINFOS;
    return sendOnSocket(sockfd_, _serialized);
}

void Client::receiveManagerInfos(int *nbPlayers, int * money, int * nbFans, int * actionPoints){
    askForManagerInfos(); //on contacte le serveur
    //écrit les données sur les objets pointés
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    if(_serialized.typeOfInfos == MANAGERINFOS){
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

int Client::askForBuildingInfos(int buildingID){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = GETBUILDINGINFOS;
    memcpy(position, &buildingID, sizeof(buildingID));
    return sendOnSocket(sockfd_, _serialized);
}

std::vector<int> Client::receiveBuildingInfos(int buildingID){
    askForBuildingInfos(buildingID);
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    std::vector<int> buildingInfos;
    if(_serialized.typeOfInfos == BUILDINGINFOS){ //on suppose toujours vrai
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

int Client::askForBuildingUpgrade(int buildingID){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = UPGRADE_BUILDING;
    memcpy(position, &buildingID, sizeof(buildingID));
    return sendOnSocket(sockfd_, _serialized);
}

int Client::askForPlayersList(){
    _serialized.typeOfInfos = GETPLAYERSLIST;
    return sendOnSocket(sockfd_, _serialized);
}

std::vector<std::string> Client::receivePlayersList(){
    askForPlayersList();
    //pour chaque ManagedPlayer, on reçoit un string firstname et un lastname, de façon ordonnée (indice 0 à 1 : playerID = 0, etc)
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    std::vector<std::string> playersListNames;
    if(_serialized.typeOfInfos == PLAYERSLIST){ //on suppose toujours vrai
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

int Client::askForPlayerInfos(int playerID){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = GETPLAYERINFOS;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, _serialized);
}

std::vector<int> Client::receivePlayerInfo(int playerID){
    askForPlayerInfos(playerID);
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    std::vector<int> playerInfos;
    if(_serialized.typeOfInfos == PLAYERINFOS){ //on suppose toujours vrai
            //5 attributs int
			//5 états d'entrainements d'attribut int
			//1 int blocked
			//1 int bonus du balais
			//1 int capacity du balais
            //1 int life
            //1 int estimated value of player
        playerInfos = receiveIntList(position, 15);
    }
    return playerInfos;
}

int Client::trainPlayer(int playerID, int capacity){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = TRAIN_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &capacity, sizeof(capacity));
    return sendOnSocket(sockfd_, _serialized);
}

int Client::healPlayer(int playerID){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = HEAL_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, _serialized);
}

//match\\

int Client::getManagersList(){
    _serialized.typeOfInfos = GETMANAGERSLIST;
    return sendOnSocket(sockfd_, _serialized);
}

void Client::receiveManagersIDandNames(std::vector<int> * IDList, std::vector<std::string> * namesList){
    getManagersList();
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
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

int Client::proposeMatchTo(int userID, std::vector<int> playersInTeam){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = PROPOSEMATCH;
    memcpy(position, &userID, sizeof(userID));
    position += sizeof(userID);
    serializeTeam(position, playersInTeam);
    
    return sendOnSocket(sockfd_, _serialized);
}

int Client::answerMatchProposal(bool confirmation, std::vector<int> playersInTeam){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = ACCEPTMATCH;
    memcpy(position, &confirmation, sizeof(confirmation));
    position += sizeof(confirmation);
    
    serializeTeam(position, playersInTeam);
    return sendOnSocket(sockfd_, _serialized);
}

void Client::serializeTeam(char * position, std::vector<int> playersInTeam){
    int value;
    for(unsigned int i = 0; i < playersInTeam.size();++i){ //joueurs choisis pour jouer le match
        value = playersInTeam[i];
        memcpy(position, &value, sizeof(value));
        position += sizeof(value);
    }
}

int Client::sendTeamForMatchTournament(std::vector<int> playersInTeam){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = STARTTOURNAMENTMATCH;
    serializeTeam(position, playersInTeam);
    return sendOnSocket(sockfd_, _serialized);
}

int Client::sendTrainingMatchRequest(std::vector<int> playersInTeam){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = PLAYTRAININGMATCH;
    serializeTeam(position, playersInTeam);
    return sendOnSocket(sockfd_, _serialized);
}

int Client::receiveMatchConfirmation(){
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    int confirmation;
    memcpy(&confirmation, position, sizeof(confirmation));
    return confirmation;
}

int Client::getAllPositions(){
    _serialized.typeOfInfos = GETPOSITIONS;
    return sendOnSocket(sockfd_, _serialized);
}

std::vector<AxialCoordinates> Client::receiveScoresAndPositions(int * winner, int * scoreTeam1, int * scoreTeam2){
    getAllPositions();
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    std::vector<AxialCoordinates> orderedPositions;
    int diag;
    int line;
    memcpy(winner, position, sizeof(int));
    position += sizeof(int);
    memcpy(scoreTeam1, position, sizeof(int));
    position += sizeof(int);
    memcpy(scoreTeam2, position, sizeof(int));
    position += sizeof(int);
    std::vector<int> coordList = receiveIntList(position, 36); //18 coord de 2 entiers
    for(unsigned int i = 0; i < 36; i+=2){ 
        //~ memcpy(&diag, position, sizeof(diag));
        //~ position += sizeof(diag);
        //~ memcpy(&line, position, sizeof(line));
        //~ position += sizeof(line);
        orderedPositions.push_back(AxialCoordinates(coordList[i], coordList[i+1]));
    }
    return orderedPositions;
}

std::vector<int> Client::receiveIntList(char * position, int nbToRead){
    std:vector<int> list;
    int read;
    for(int i = 0; i < nbToRead; ++i){
        memcpy(&read, position, sizeof(read));
        position += sizeof(read);
        list.push_back(read);
    }
    return list;
}

int Client::selectPlayer(int playerID){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = SELECTPLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, _serialized);
}

playerAttr Client::receiveSelectedPlayerInfos(int playerID){
    selectPlayer(playerID);
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    playerAttr thePlayer;
    int attribute, diag, line;
    for(int i = 0; i < 5; ++i){
        memcpy(&attribute, position, sizeof(attribute));
        position += sizeof(attribute);
        thePlayer.attributes[i] = attribute;
    }
    memcpy(&diag, position, sizeof(diag)); //coord 1 : diagonale
    position += sizeof(diag);
    memcpy(&line, position, sizeof(line)); //coord 2 : ligne
    position += sizeof(line);
    thePlayer.position = AxialCoordinates(diag, line);
    memcpy(&attribute, position, sizeof(attribute)); //hasQuaffle
    position += sizeof(attribute);
    thePlayer.hasQuaffle = attribute;
    memcpy(&attribute, position, sizeof(attribute)); //hasQuaffle
    position += sizeof(attribute);
    thePlayer.life = attribute;
    return thePlayer;
}

int Client::sendMoves(int moves[][4]){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = MAKEMOVES;
    int targetedPlayer;
    int specialAction;
    int diagDest;
    int lineDest;
    for(int i = 0; i < 7; ++i){
        targetedPlayer = moves[i][0];
        specialAction = moves[i][1];
        diagDest = moves[i][2];
        lineDest = moves[i][3];
        printf("%s : %d %d %d\n", "playerID diagDest lineDest", targetedPlayer, diagDest, lineDest);
        memcpy(position, &targetedPlayer,sizeof(targetedPlayer)); 
        position += sizeof(targetedPlayer);
        memcpy(position, &specialAction,sizeof(specialAction)); 
        position += sizeof(specialAction);
        memcpy(position, &diagDest, sizeof(diagDest));
        position += sizeof(diagDest); 
        memcpy(position, &lineDest, sizeof(lineDest));
        position += sizeof(lineDest); 
    }
    return sendOnSocket(sockfd_, _serialized);
}



//useless
//int Client::isMatchWaiting(){
//    SerializedObject _serialized;
//    char * position = _serialized.stringData;
//    _serialized.typeOfInfos = IS_MATCH_WAITING;
//    return sendOnSocket(sockfd_, _serialized);
//}

//auctions\\

int Client::sellPlayer(int playerID, int startingPrice){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = CREATEAUCTION;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &startingPrice, sizeof(startingPrice));
    return sendOnSocket(sockfd_, _serialized);
}

int Client::getAuctionsList(){
    _serialized.typeOfInfos = GETAUCTIONSLIST;
    return sendOnSocket(sockfd_, _serialized);
}

std::vector<std::string> Client::receiveAuctionsList(){
    getAuctionsList();
    //pour chaque ManagedPlayer, on reçoit un string firstname et un lastname, de façon ordonnée (indice 0 à 1 : playerID = 0, etc)
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    std::vector<std::string> auctionsList;
    if(_serialized.typeOfInfos == AUCTIONSLIST){ //on suppose toujours vrai
        int nbInfos;
        memcpy(&nbInfos,position, sizeof(nbInfos));
        position += sizeof(nbInfos);
        for(int i = 0; i < nbInfos; ++i){
            char info[2*USERNAME_LENGTH];
            std::string strInfos;
            memcpy(&info,position, sizeof(info));
            position += sizeof(info);
            strInfos = info; //conversion
            auctionsList.push_back(strInfos); //ajout à la liste
        }
    }
    return auctionsList;
}

int Client::joinAuction(int auctionID){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = JOINAUCTION;
    memcpy(position, &auctionID, sizeof(auctionID));
    return sendOnSocket(sockfd_, _serialized);
}

int Client::askForAuctionInfos(int auctionID){
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = GETAUCTIONINFO;
    memcpy(position, &auctionID, sizeof(auctionID));
    return sendOnSocket(sockfd_, _serialized);
}

int Client::getCurrentPrice(){
    int result;
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    memcpy(&result,position, sizeof(result));
    return result;
}

int Client::askCurrentPrice(){
    _serialized.typeOfInfos = GET_AUCTION_PRICE;
    return sendOnSocket(sockfd_, _serialized);
}

int Client::bid(){
    _serialized.typeOfInfos = BID;
    return sendOnSocket(sockfd_, _serialized);
}

int Client::checkAuction(){
    _serialized.typeOfInfos = END_AUCTION_TURN;
    return sendOnSocket(sockfd_, _serialized);
}

int Client::receiveAuctionResult() {
    int result;
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    memcpy(&result,position, sizeof(result));
    return result;
}

int Client::askAuctionTimeLeft(){
    _serialized.typeOfInfos = GET_AUCTION_TIME_LEFT;
    return sendOnSocket(sockfd_, _serialized);
}
int Client::getAuctionTimeLeft(){
    int result;
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    memcpy(&result,position, sizeof(result));
    return result;
}


int Client::startPromotionCampaign(){
    _serialized.typeOfInfos = START_PROMOTION;
    return sendOnSocket(sockfd_, _serialized);
}

int Client::endPromotionCampaign(){
    _serialized.typeOfInfos = END_PROMOTION;
    return sendOnSocket(sockfd_, _serialized);
}

int Client::getPromotionResult() {
    int result;
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    memcpy(&result,position, sizeof(result));
    return result;
}

int Client::buyActionPoints(int amount) {
    char * position = _serialized.stringData;
    _serialized.typeOfInfos = BUY_ACTION_POINTS;
    memcpy(position, &amount, sizeof(amount));
    return sendOnSocket(sockfd_, _serialized);
}

int Client::getPriceForAP(){    
    int result;
    _serialized = receiveOnSocket(sockfd_);
    char * position = _serialized.stringData;
    memcpy(&result,position, sizeof(result));
    return result;
}

int Client::askToJoinTournament(int tournamentID){
    _serialized.typeOfInfos = JOINTOURNAMENT;
    char * position = _serialized.stringData;
    memcpy(position, &tournamentID, sizeof(tournamentID));
    return sendOnSocket(sockfd_, _serialized);
}




string intToString(int value) {
    char buffer[800];
    sprintf(buffer,"%d",value);
    string tmp = "";
    int i = 0;
    while (buffer[i] != '\0') {
        tmp+=buffer[i];
        ++i;
    }
    return tmp;
}
