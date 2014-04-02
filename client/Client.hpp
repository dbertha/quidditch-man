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



typedef struct { //version simplifiée de la classe sur le serveur
    int attributes[5];
    AxialCoordinates position;
    int hasQuaffle;
    int life;
} playerAttr;

class Client {
public:
    Client(int);
    //for the thread :
    int getConfirmation();
    int bid();
    int getCurrentPrice();
    int askCurrentPrice();

private:
    int sockfd_; //socket d'écoute du serveur
    SerializedObject _serialized;
    int _bufferPosition;
    int askForManagerInfos();
    int askForBuildingInfos(int buildingID);
    int askForPlayersList();
    int getManagersList();
    int askForPlayerInfos(int playerID);
    void serializeTeam(char * buffer, std::vector<int> playersInTeam);
    int getAllPositions();
    int selectPlayer(int playerID);
    int getAuctionsList();
public :
    int getSockfd() const {return sockfd_;}

    int sendLoginToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH], bool newUser = false);
    int sendNewManagerToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]);
    
    
    
    void receiveManagerInfos(int *nbPlayers, int * money, int * nbFans, int * actionPoints);
    
    
    std::vector<int> receiveBuildingInfos(int buildingID);
    int askForBuildingUpgrade(int buildingID);
    
    std::vector<std::string> receivePlayersList();
    
    std::vector<int> receivePlayerInfo(int playerID);
    int trainPlayer(int playerID, int capacity);
    int healPlayer(int playerID);
    
    
    void receiveManagersIDandNames(std::vector<int> * IDList, std::vector<std::string> * namesList);
    int proposeMatchTo(int userID, std::vector<int> playersInTeam);
    int answerMatchProposal(bool confirmation, std::vector<int> playersInTeam);
    int sendTeamForMatchTournament(std::vector<int> playersInTeam);
    int receiveMatchConfirmation();
    int sendTrainingMatchRequest(std::vector<int> playersInTeam);
    
    std::vector<AxialCoordinates> receiveScoresAndPositions(int * winner, int * scoreTeam1, int * scoreTeam2);
    
    playerAttr receiveSelectedPlayerInfos(int playerID);
    int sendMoves(int moves[][4]); 
    int sendForfeit(); 
    int sendDrawRequest();
    int sendAnswerToDrawProposition(int code);
    
    int sellPlayer(int playerID, int startingPrice);
    
    std::vector<std::string> receiveAuctionsList();
    int joinAuction(int auctionID);
    int askForAuctionInfos(int auctionID);


    int checkAuction();
    int receiveAuctionResult();

    int askAuctionTimeLeft();
    int getAuctionTimeLeft();

    int buyActionPoints(int amount);
    int getPriceForAP();

    int startPromotionCampaign();
    int endPromotionCampaign();
    int getPromotionResult();
    
    int sendTournamentCreation(int nbOfPlayers, int startingPrice);
    int askForTournamentList();
    std::vector<int> getTournamentList();
    int askToJoinTournament(int tournamentID = 0);
    int receiveNumOfTeam();
};

std::string intToString(int value);

#endif
