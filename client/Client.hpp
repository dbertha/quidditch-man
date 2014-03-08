#ifndef CLIENT_H
#define CLIENT_H

#include "../common/Defines.hpp"
#include "../common/NetworkBase.hpp"
#include "../common/Coordinates.hpp"
#include "../common/HexagonalField.hpp"
//#include "../common/NetworkInterface.hpp"
#include <iostream>
#include <set>
#include <vector>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>



#define DIM 100

#define LOG_IN 1
#define NEW_MANAGER 2
#define SEE_MANAGERS 1
#define AUCTION_ROOM 2
#define MANAGE_PLAYERS 3
#define MANAGE_BUILDINGS 4

#define SEE_AUCTIONS 1
#define SELL_PLAYER 2
#define INSPECT_PLAYER 1
#define TRAIN_PLAYER_OPTION 2
#define HEAL_PLAYER_OPTION 3

#define ENTER_STADIUM 1
#define ENTER_TRAININGCENTER 2
#define ENTER_HOSPITAL 3
#define ENTER_FANSHOP 4

#define SEE_TOURNAMENTS 1
#define CREATE_TOURNAMENT_OPTION 2

#define ABORT 0

#define TIMESCALE 1

typedef struct { //pas besoin de la classe complète
    int attributes[5];
    AxialCoordinates position;
    int hasQuaffle;
} playerAttr;

class Client {
public:
    Client(int, bool GUI = false);
    void run();
    //for the thread :
    int getConfirmation();
    int bid();

private:
    bool GUI_;
    int sockfd_; //socket d'écoute du serveur
//SerializedObject received;
    //char msg[INPUTSIZE];
    int input_;
    char opponent_[10];
    fd_set FDSet_;
    enum Status {INIT,ADMIN,FREE,MANAGERS_MENU, AUCTION_MENU, PLAYERS_MENU, BUILDINGS_MENU, PLAYERSLIST_MENU, TRAINING_MENU, HEALING_MENU, MATCH_LIST,MATCH_INVITING,MATCH_INVITED,MATCH_INGAME,DISCONNECTING};
    Status state_;

    int mainLoop();
    void loadFDSet();
    bool keyboard();
    void askInput();
    //~ void login();
    //display :
    void displayMainMenu();
    void displayAdminMenu();
    void displayConnexionMenu();
    void displayManagerInfos();
    void displayPlayersList();
    void displayBuildingInfos(std::vector<int> buildingInfos, int buildingID);
    void displayPlayerInfos(std::vector<int> playerInfos, int playerID);
    std::string displayAuctionInfos(std::vector<std::string> auctionsList,std::vector<int> playerInfos, int auctionID);
    void displayAuctionMenus();
    void displaySellPlayerMenu();
    void displayManagePlayersMenu();
    void displayManageBuildingsMenu();
    void displayAvailableManagers();
    std::vector<int> displayAndAskPlayersForMatch();
    int testifContinue(int numTeam); //combo display + select + handler pour la poursuite d'un match
    //handlers :
    void kbMgr();
    void handleLogin();
    void handleMainMenu();
    void handleAdminMenu();
    void handleOpponentChoice();
    void handleAuctions();
    void handlePlayersMenu();
    void mainAuction(int auctionID, int timeLeft);
    //void *auctionTurn(void* data); : extern
    void startMatch(int numTeam);
    void askAndSendMoves(int numTeam, HexagonalField &field, std::vector<AxialCoordinates> &positions);
    void commMgr();
    void matchTentative();
    void contactServer();
    void receiveMessage();
    //network :
    int sendLoginToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
    int sendNewManagerToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
    
    
    int askForManagerInfos();
    void receiveManagerInfos(int *nbPlayers, int * money, int * nbFans);
    int askForBuildingInfos(int buildingID);
    std::vector<int> receiveBuildingInfos();
    int askForBuildingUpgrade(int buildingID);
    int askForPlayersList();
    std::vector<std::string> receivePlayersList();
    int askForPlayerInfos(int playerID);
    std::vector<int> receivePlayerInfo();
    int trainPlayer(int playerID, int capacity);
    int healPlayer(int playerID);
    
    int getManagersList();
    void receiveManagersIDandNames(std::vector<int> * IDList, std::vector<std::string> * namesList);
    int proposeMatchTo(int userID, std::vector<int> playersInTeam);
    int answerMatchProposal(bool confirmation, std::vector<int> playersInTeam);
    int receiveMatchConfirmation();
    int getAllPositions();
    std::vector<AxialCoordinates> receiveScoresAndPositions(int * winner, int * scoreTeam1, int * scoreTeam2);
    int selectPlayer(int playerID);
    playerAttr receiveSelectedPlayerInfos();
    int sendMoves(int moves[][4]); //TODO : moves confirmation ?
    int sendForfeit();
    int sendDrawRequest();
    int sendAnswerToDrawProposition(int code);
    
    int sellPlayer(int playerID, int startingPrice);
    int getAuctionsList();
    std::vector<std::string> receiveAuctionsList();
    int joinAuction(int auctionID);
    int askForAuctionInfos(int auctionID);
    int getCurrentPrice();
    
    int checkAuction();
    int receiveAuctionResult();
    
    int sendTournamentCreation(int nbOfPlayers, int startingPrice);
};



#endif
