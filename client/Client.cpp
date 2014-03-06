#include "Client.hpp"
#include "commAPI.hpp"
// Documentation : voir Readme.txt

using namespace std; //TODO : rajouter tous les spécificateurs de namespace

Client::Client(int sockfd): sockfd_(sockfd), state_(INIT) {}
// initialisations dans le constructeur
void Client::run() {
    mainLoop();
    close(sockfd_);
}

int Client::mainLoop() {
    while(state_!=DISCONNECTING) {
        askInput();
        loadFDSet();
        if (select(sockfd_+1,&FDSet_,NULL,NULL,NULL)==ERROR) {
            std::cerr<<"Socket bind error"<<std::endl;
            return EXIT_FAILURE;
        }
        if(keyboard()) kbMgr();
        else commMgr();
    }
    return EXIT_SUCCESS;
}

///////Display\\\\\\\

//Main\\

void Client::displayManagerInfos() {
    int nbPlayers, money, nbFans;
    askForManagerInfos();
    receiveManagerInfos(&nbPlayers,&money,&nbFans);
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<" -------------------- MONEY : "<<money<<" ||| "<<" NUMBER OF FANS : "<<nbFans<<" --------------------"<<endl;
}

void Client::displayMainMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"What do you want to do ?"<<endl;
    cout<<" [1] See managers connected to propose match"<<endl;
    cout<<" [2] See/create auctions"<<endl;
    cout<<" [3] Manage players"<<endl;
    cout<<" [4] Manage buildings"<<endl;
    cout<<" [0] Quit game"<<endl;
    cout<<"-----> ";
}

//Log in\\

void Client::displayConnexionMenu(){
    cout<<" ------------------------ WELCOME TO QUIDDITCH MANAGER 2014 ! ------------------------"<<endl;
    cout<<" [1] Log in"<<endl;;
    cout<<" [2] Create a new manager"<<endl;
    cout<<"-----> ";
}

//Management\\

void Client::displayManagePlayersMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//MANAGE TEAM// What do you want to do ?"<<endl;
    cout<<" [1] Inspect a player"<<endl;
    cout<<" [2] Train a player"<<endl;
    cout<<" [3] Send a player to the hospital"<<endl;
    cout<<" [0] Return to the main menu"<<endl;
    cout<<"-----> ";
}

void Client::displayManageBuildingsMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//MANAGE BUILDINGS// What do you want to do ?"<<endl;
    cout<<" [1] Enter Stadium"<<endl;
    cout<<" [2] Enter Training Center"<<endl;
    cout<<" [3] Enter Hospital"<<endl;
    cout<<" [4] Enter FanShop"<<endl;
    cout<<" [0] Return to the main menu"<<endl;
    cout<<"-----> ";
}

void Client::displayPlayersList() {
    askForPlayersList();
    vector<string> playersList = receivePlayersList();
    for (unsigned int i=0;i<playersList.size();i+=2) {cout<<" ["<<(i+2)/2<<"] "<<playersList[i]<<" "<<playersList[i+1]<<endl;}
}

void Client::displayBuildingInfos(std::vector<int> buildingInfos, int buildingID){
    cout<<"Current level : "<<buildingInfos[0]<<endl;
    cout<<"Price for next level : "<<buildingInfos[1]<<endl;
    if (buildingID==STADIUM) cout<<"Max places in the Stadium : "<<buildingInfos[2]<<endl;
    else if (buildingID==TRAININGCENTER) cout<<"Time required to finish training : "<<buildingInfos[2]<<endl;
    else if (buildingID==HOSPITAL) cout<<"Time required to finish healing : "<<buildingInfos[2]<<endl;
    else if (buildingID==FANSHOP) cout<<"Max clients in the fan shop per match : "<<buildingInfos[2]<<endl;
    if (buildingInfos[3]==1) cout<<"This building is currently being upgraded"<<endl;
    else cout<<"This building is not upgrading yet.";
}

void Client::displayPlayerInfos(vector<int> playerInfos, int playerID) {
    askForPlayersList();
    std::vector<string> playersList = receivePlayersList();
    cout<<"-------- "<<playersList[2*playerID]<<" "<<playersList[2*playerID+1]<<" --------"<<endl;
    cout<<" [1] Speed : "<<playerInfos[0]<<endl;
    cout<<" [2] Strength : "<<playerInfos[1]<<endl;
    cout<<" [3] Precision : "<<playerInfos[2]<<endl;
    cout<<" [4] Reflex : "<<playerInfos[3]<<endl;
    cout<<" [5] Resistance : "<<playerInfos[4]<<endl;
    cout<<"---------------------------------\n Number of trainings left to up speed : "<<playerInfos[5]<<endl;
    cout<<" Number of trainings left to up strength : "<<playerInfos[6]<<endl;
    cout<<" Number of trainings left to up precision : "<<playerInfos[7]<<endl;
    cout<<" Number of trainings left to up reflex : "<<playerInfos[8]<<endl;
    cout<<" Number of trainings left to up resistance : "<<playerInfos[9]<<endl;
    if (playerInfos[10]==1) cout<<"\n This player is currently blocked."<<endl;
    else cout<<"\n This player isn't blocked."<<endl;
    if (playerInfos[12]>0) {
        cout<<" The broomstick of this player grants a bonus of "<<playerInfos[12]<<" for ";
        if (playerInfos[11]==0) cout<<"speed"<<endl;
        else if (playerInfos[11]==1) cout<<"strength"<<endl;
        else if (playerInfos[11]==2) cout<<"precision"<<endl;
        else if (playerInfos[11]==3) cout<<"reflexx"<<endl;
        else if (playerInfos[11]==4) cout<<"resistance"<<endl;
    }
    else cout<<" The broomstick of this player doesn't grant him any bonus."<<endl;
  
    cout<<" The life of this player is "<<playerInfos[13]<<" (max="<<playerInfos[4]<<")"<<endl;
    cout<<" The value of this player is estimated at "<<playerInfos[14]<<" gold"<<endl;
}

//Auctions\\

void Client::displayAuctionMenus(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//YOU'RE NOW IN THE AUCTION ROOM// What do you want to do ?"<<endl;
    cout<<" [1] See current auctions"<<endl;
    cout<<" [2] Sell a player"<<endl;
    cout<<" [0] Return to the main menu"<<endl;
    cout<<"-----> ";
}

void Client::displaySellPlayerMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//YOU'RE ABOUT TO SELL A PLAYER// What do you want to do ?"<<endl;
    displayPlayersList();
    cout<<" Which player do you want to sell [enter 0 to abort] ? "<<endl;
}


std::string Client::displayAuctionInfos(vector<string> auctionsList,vector<int> playerInfos, int auctionID) {
    std::string name;
    std::string price;
    std::string timeLeft;
    for (unsigned int i=0;i<auctionsList.size();i+=4) {
        if (atoi(auctionsList[i].c_str())==auctionID) {
            name = auctionsList[i+1];
            price = auctionsList[i+3];
            timeLeft = auctionsList[i+2];
        }
    }
    cout<<"-------- "<<name<<" --------"<<endl;
    cout<<" [1] Speed : "<<playerInfos[0]<<endl;
    cout<<" [2] Strength : "<<playerInfos[1]<<endl;
    cout<<" [3] Precision : "<<playerInfos[2]<<endl;
    cout<<" [4] Reflex : "<<playerInfos[3]<<endl;
    cout<<" [5] Resistance : "<<playerInfos[4]<<endl;
    cout<<"---------------------------------\n Number of trainings left to up speed : "<<playerInfos[5]<<endl;
    cout<<" Number of trainings left to up strength : "<<playerInfos[6]<<endl;
    cout<<" Number of trainings left to up precision : "<<playerInfos[7]<<endl;
    cout<<" Number of trainings left to up reflex : "<<playerInfos[8]<<endl;
    cout<<" Number of trainings left to up resistance : "<<playerInfos[9]<<endl;
    if (playerInfos[10]==1) cout<<"\n This player is currently blocked."<<endl;
    else cout<<"\n This player isn't blocked."<<endl;
    if (playerInfos[12]>0) {
        cout<<" The broomstick of this player grants a bonus of "<<playerInfos[12]<<" for ";
        if (playerInfos[11]==0) cout<<"speed"<<endl;
        else if (playerInfos[11]==1) cout<<"strength"<<endl;
        else if (playerInfos[11]==2) cout<<"precision"<<endl;
        else if (playerInfos[11]==3) cout<<"reflex"<<endl;
        else if (playerInfos[11]==4) cout<<"resistance"<<endl;
    }
    else cout<<" The broomstick of this player doesn't grant him any bonus."<<endl;
  
    cout<<" The life of this player is "<<playerInfos[13]<<" (max="<<playerInfos[4]<<")"<<endl;
    cout<<" The value of this player is estimated at "<<playerInfos[14]<<" gold"<<endl;
    cout<<"\n STARTING PRICE FOR THE AUCTION : "<<price<<endl;
  
    return timeLeft;
}

void displayAuctionsList(vector<string> auctionsList) {
    if (auctionsList.size()==0) cout<<" --------- No auction available"<<endl;
    else {
        for (unsigned int i=0;i<auctionsList.size();i+=4) {
            cout<<" ["<<atoi(auctionsList[i].c_str())+1<<"] "<<auctionsList[i+1]<<" | Starting price : "<<auctionsList[i+3];
            cout<<" | Time left to participate : "<<auctionsList[2]<<endl;
        }
        cout<<"Indicate which auction you wish to inspect [or 0 to abort] : ";
    }
}



//Match\\

////////handling interactions :\\\\\\\

void Client::askInput() {
    switch(state_) {
        case INIT : {
            displayConnexionMenu();
            break;
        }
        case FREE : {
            displayMainMenu();
            break;
        }
        default : {
            std::cout<<"Aucune option pour l'instant."<<std::endl;
        }
    }
}

void Client::loadFDSet() {
//charge le set des FD que doit surveiller en lecture accept()
//comme select() écrase cet input par la liste des FD actifs,
//on exécute cette fonction avant chaque select()
    FD_ZERO(&FDSet_); //l'ensemble est vidé
    FD_SET(sockfd_,&FDSet_); //le socket
    FD_SET(STDIN_FILENO,&FDSet_); //le file descriptor de l'input pour accepter des commandes du clavier
}

bool Client::keyboard() {return FD_ISSET(STDIN_FILENO,&FDSet_);}

void Client::login() {
    contactServer();
    state_=FREE;
}



//~ void Client::displayMenu() {
        //~ std::cout<<"Que souhaitez-vous faire ?"<<std::endl;
        //~ std::cout<<"Liste des managers disponibles pour un match amical"<<std::endl;
        //~ std::cout<<"Gestion de vos bâtiments"<<std::endl;
        //~ std::cout<<"Consultation des enchères"<<std::endl;
        //~ std::cout<<"Mise aux enchères d'un joueur"<<std::endl;
        //~ std::cout<<"Arrêt du programme"<<std::endl;
//~ 
//~ }

void Client::kbMgr() {
    std::cin>>input_;
    switch(state_) {
        case INIT : {
            //contexte verrouillé, pas d'écoute du socket tant que login pas terminé
            char username[USERNAME_LENGTH];
            char password[PASSWORD_LENGTH];
            std::cout<<"Enter login : ";
            std::cin>>username;
            std::cout<<"Enter password : ";
            std::cin>>password;
            if (input_==LOG_IN) sendLoginToServer(username,password);
            else if (input_==NEW_MANAGER) sendNewManagerToServer(username,password);
            int result = getConfirmation();
            if (result == LOGIN_FAILED) {
                if (input_==LOG_IN) cout<<" ------------- WRONG LOGIN/PASSWORD"<<endl;
                else if (input_==NEW_MANAGER) cout<<" ------------- LOGIN ALREADY TAKEN"<<endl;
            }
            else{
                if(result == NORMAL_LOGIN){
                    state_ = FREE;
                }
                else{
                    //ADMIN_LOGIN
                    state_ = ADMIN;
                }
            }
            break;
        }
        case FREE : {
            
        //affichage du sous-menu avec adaptation du contexte
            
            break;
        }
        default : {//ne devrait jamais passer par ici
            std::cout<<"Je ne puis rien faire pour vous."<<std::endl;
        }
    }
}

void Client::commMgr() {
//gère les messages non sollicités (exemple : invitation à un match amical)

}

void Client::matchTentative() {
//    msg[0]='M';
//    msg[1]=' ';
//        std::cout<<"Managers disponibles :"<<std::endl;
//        contactServer();
//        if(msg[1]==' ') {//il y a des managers disponibles
//                state_=MATCH_INVITING;
//                std::cin>>input_; //nom du manager choisi comme adversaire
//                msg[1]='S';
//                strcpy(opponent_,input_);
//                strcpy(&msg[2],opponent_);
//                contactServer();
//                if(msg[1]==' ') //le match peut commencer
//                        state_=MATCH_INGAME;
//                        std::cout<<"C'est parti pour un match amical !"<<std::endl;
//// le nom de mon adversaire se trouve dans opponent_
////                      startGame()
//                state_=FREE;
//        }
}

void Client::contactServer() {
        //sendMsg(sockfd_,msg,strlen(msg));
        //receiveMessage();
}
void Client::receiveMessage() {
        //receiveMsg(sockfd_,msg,INPUTSIZE);
        //std::cout<<&msg[2]<<std::endl;
}

////////Network\\\\\\\\

//login\\

int Client::sendLoginToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]){
    //REM : les param reçu ne sont pas des char[N] (sizeof() = N) mais des char * (sizeof() = taille pointeur)
    SerializedObject serialized;
    char * position;
    serialized.typeOfInfos = LOGIN;
    position = serialized.stringData;
    memcpy(position, username, USERNAME_LENGTH);
    position += USERNAME_LENGTH;
    memcpy(position, password, PASSWORD_LENGTH);
    //position += sizeof(password);
    return sendOnSocket(sockfd_, serialized);
}

int Client::sendNewManagerToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]){
    //REM : les param reçu ne sont pas des char[N] (sizeof() = N) mais des char * (sizeof() = taille pointeur)
    //TODO : redondance avec sendLogin
    SerializedObject serialized;
    char * position;
    serialized.typeOfInfos = CREATE_MANAGER;
    position = serialized.stringData;
    memcpy(position, username, USERNAME_LENGTH);
    position += USERNAME_LENGTH;
    memcpy(position, password, PASSWORD_LENGTH);
    //position += sizeof(password);
    return sendOnSocket(sockfd_, serialized);
}

int Client::getConfirmation(){ //valable pour LOGIN_CONFIRM, UPGRADE_CONFIRM, TRAINING_STARTED, HEAL_STARTED, AUCTIONCREATION_CONFIRM
    int confirmation;
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    //TODO : vérifier qu'il s'agit bien d'un message de confirmation
    memcpy(&confirmation,position, sizeof(confirmation));
    return confirmation;
}

//management\\

int Client::askForManagerInfos(){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETMANAGERINFOS;
    return sendOnSocket(sockfd_, serialized);
}

void Client::receiveManagerInfos(int *nbPlayers, int * money, int * nbFans){
    //écrit les données sur les objets pointés
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    if(received.typeOfInfos == MANAGERINFOS){
        memcpy(nbPlayers,position, sizeof(int));
        position += sizeof(int);
        memcpy(money,position, sizeof(int));
        position += sizeof(int);
        memcpy(nbFans,position, sizeof(int));
        position += sizeof(int);
    }
}

int Client::askForBuildingInfos(int buildingID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETBUILDINGINFOS;
    memcpy(position, &buildingID, sizeof(buildingID));
    return sendOnSocket(sockfd_, serialized);
}

std::vector<int> Client::receiveBuildingInfos(){
    SerializedObject received = receiveOnSocket(sockfd_);
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

int Client::askForBuildingUpgrade(int buildingID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = UPGRADE_BUILDING;
    memcpy(position, &buildingID, sizeof(buildingID));
    return sendOnSocket(sockfd_, serialized);
}

int Client::askForPlayersList(){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETPLAYERSLIST;
    return sendOnSocket(sockfd_, serialized);
}

std::vector<std::string> Client::receivePlayersList(){
    //pour chaque ManagedPlayer, on reçoit un string firstname et un lastname, de façon ordonnée (indice 0 à 1 : playerID = 0, etc)
    SerializedObject received = receiveOnSocket(sockfd_);
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

int Client::askForPlayerInfos(int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETPLAYERINFOS;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, serialized);
}

std::vector<int> Client::receivePlayerInfo(){
    SerializedObject received = receiveOnSocket(sockfd_);
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

int Client::trainPlayer(int playerID, int capacity){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = TRAIN_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &capacity, sizeof(capacity));
    return sendOnSocket(sockfd_, serialized);
}

int Client::healPlayer(int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = HEAL_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, serialized);
}

//match\\

int Client::getManagersList(){
    SerializedObject serialized;
    serialized.typeOfInfos = GETMANAGERSLIST;
    return sendOnSocket(sockfd_, serialized);
}

void Client::receiveManagersIDandNames(std::vector<int> * IDList, std::vector<std::string> * namesList){
    SerializedObject received = receiveOnSocket(sockfd_);
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

int Client::proposeMatchTo(int userID, std::vector<int> playersInTeam){
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
    
    return sendOnSocket(sockfd_, serialized);
}

int Client::answerMatchProposal(bool confirmation, std::vector<int> playersInTeam){
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
    return sendOnSocket(sockfd_, serialized);
}

int Client::receiveMatchConfirmation(){
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    int confirmation;
    memcpy(&confirmation, position, sizeof(confirmation));
    return confirmation;
}

int Client::getAllPositions(){
    SerializedObject serialized;
    serialized.typeOfInfos = GETPOSITIONS;
    return sendOnSocket(sockfd_, serialized);
}

std::vector<AxialCoordinates> Client::receiveScoresAndPositions(int * winner, int * scoreTeam1, int * scoreTeam2){
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    std::vector<AxialCoordinates> orderedPositions;
    int diag;
    int line;
    memcpy(winner, position, sizeof(int));
    position += sizeof(int);
    memcpy(scoreTeam1, position, sizeof(int));
    position += sizeof(int);
    memcpy(scoreTeam2, position, sizeof(int));
    position += sizeof(int);
    for(unsigned int i = 0; i < 18; ++i){ //positions des 18 objets
        memcpy(&diag, position, sizeof(diag));
        position += sizeof(diag);
        memcpy(&line, position, sizeof(line));
        position += sizeof(line);
        orderedPositions.push_back(AxialCoordinates(diag, line));
    }
    return orderedPositions;
}

int Client::selectPlayer(int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = SELECTPLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, serialized);
}

playerAttr Client::receiveSelectedPlayerInfos(){
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
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
    return thePlayer;
}

int Client::sendMoves(int moves[][4]){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = MAKEMOVES;
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
    return sendOnSocket(sockfd_, serialized);
}

//TODO : useless
//int Client::isMatchWaiting(){
//    SerializedObject serialized;
//    char * position = serialized.stringData;
//    serialized.typeOfInfos = IS_MATCH_WAITING;
//    return sendOnSocket(sockfd_, serialized);
//}

//auctions\\

int Client::sellPlayer(int playerID, int startingPrice){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = CREATEAUCTION;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &startingPrice, sizeof(startingPrice));
    return sendOnSocket(sockfd_, serialized);
}

int Client::getAuctionsList(){
    SerializedObject serialized;
    serialized.typeOfInfos = GETAUCTIONSLIST;
    return sendOnSocket(sockfd_, serialized);
}

std::vector<std::string> Client::receiveAuctionsList(){
    //pour chaque ManagedPlayer, on reçoit un string firstname et un lastname, de façon ordonnée (indice 0 à 1 : playerID = 0, etc)
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    std::vector<std::string> auctionsList;
    if(received.typeOfInfos == AUCTIONSLIST){ //on suppose toujours vrai
        int nbInfos;
        memcpy(&nbInfos,position, sizeof(nbInfos));
        position += sizeof(nbInfos);
        std::cout << nbInfos << std::endl;
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
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = JOINAUCTION;
    memcpy(position, &auctionID, sizeof(auctionID));
    return sendOnSocket(sockfd_, serialized);
}

int Client::askForAuctionInfos(int auctionID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETAUCTIONINFO;
    memcpy(position, &auctionID, sizeof(auctionID));
    return sendOnSocket(sockfd_, serialized);
}

int Client::getCurrentPrice(){
    SerializedObject serialized;
    serialized.typeOfInfos = GETCURRENTPRICE;
    return sendOnSocket(sockfd_, serialized);
}

int Client::bid(){
    SerializedObject serialized;
    serialized.typeOfInfos = BID;
    return sendOnSocket(sockfd_, serialized);
}

int Client::checkAuction(){
    SerializedObject serialized;
    serialized.typeOfInfos = END_AUCTION_TURN;
    return sendOnSocket(sockfd_, serialized);
}

int Client::receiveAuctionResult() {
    int result;
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    //TODO : vérifier qu'il s'agit bien d'un message de confirmation
    memcpy(&result,position, sizeof(result));
    return result;
}