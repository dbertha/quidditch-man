#ifndef TERMINAL_GAME
#define TERMINAL_GAME

#include <cstring>
#include <set>
#include <vector>
#include <string> 
#include <iostream>
#include "Client.hpp"

#define DIM 100
//connexion menu :
#define LOG_IN 1
#define NEW_MANAGER 2
//main menu :
#define SEE_MANAGERS 1
#define AUCTION_ROOM 2
#define MANAGE_PLAYERS 3
#define MANAGE_BUILDINGS 4
#define ACTION_POINTS 5
#define SEE_TOURNAMENTS 6
#define START_TRAINING_MATCH 7

//auctions menu
#define SEE_AUCTIONS 1
#define SELL_PLAYER 2
//players menu :
#define INSPECT_PLAYER 1
#define TRAIN_PLAYER_OPTION 2
#define HEAL_PLAYER_OPTION 3
//buildings menu :
#define ENTER_STADIUM 1
#define ENTER_TRAININGCENTER 2
#define ENTER_HOSPITAL 3
#define ENTER_FANSHOP 4

#define ENTER_PROMOTIONCENTER 5
#define BUY_AP 1
#define WAIT_FOR_AP 2

//admin menu :
#define SEE_TOURNAMENTS_ADMIN 1
#define CREATE_TOURNAMENT_OPTION 2

#define ABORT 0

class TerminalGame {
public :
    TerminalGame(int sockfd);
    int mainLoop();
private :
    int input_;
    Client _client;
    fd_set FDSet_;
    int currentAuctionPrice_;

    enum Status {INIT,ADMIN,FREE, AVAILABLE, MANAGERS_MENU, AUCTION_MENU, PLAYERS_MENU, BUILDINGS_MENU, \
    TOURNAMENTS_MENU, PLAYERSLIST_MENU, TRAINING_MENU, HEALING_MENU, \
    MATCH_LIST,MATCH_INVITING,MATCH_INVITED,MATCH_INGAME,DISCONNECTING, \
    AP_MENU, BUY_AP_MENU, WAIT_AP_MENU};
    Status state_;
    
    
    void loadFDSet();
    bool keyboard();
    void askInput();
    
    
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
    void displayAuctionsList(std::vector<std::string> auctionsList);
    void displaySellPlayerMenu();
    void displayManagePlayersMenu();
    void displayManageBuildingsMenu();
    void displayActionPointsMenu();
    void displayAvailableManagers();
    void displayTournamentMenu();
    void displayTournamentList(std::vector<int> tournamentsList);  
    std::vector<int> displayAndAskPlayersForMatch();
    int testifContinue(int numTeam); //combo display + select + handler pour la poursuite d'un match
    //handlers :
    void kbMgr();
    void commMgr();
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
    
};

std::string intToString(int value);
void *auctionTurn(void* data);
void *auctionPrice(void* data);
#endif
