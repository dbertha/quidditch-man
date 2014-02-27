
#include "../common/NetworkBase.h"
#include "../common/NetworkInterface.hpp"
#include "../common/Defines.hpp"
#include "../common/HexagonalField.hpp"


#include <pthread.h>

//TODO : en faire une classe complète, à l'instar du server
//TODO : utiliser la méthode buildconnexion de commAPI
//TODO : actions à proposer en fonction du batiment choisi

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <time.h>


#define DIM 100

#define LOG_IN 1
#define NEW_MANAGER 2
#define SEE_MANAGERS 1
#define AUCTION_ROOM 2
#define MANAGE_PLAYERS 3
#define MANAGE_BUILDINGS 4
#define PROPOSEMATCH_OPTION 5

#define SEE_AUCTIONS 1
#define SELL_PLAYER 2
#define INSPECT_PLAYER 1
#define TRAIN_PLAYER_OPTION 2
#define HEAL_PLAYER_OPTION 3
#define ENTER_STADIUM 1
#define ENTER_TRAININGCENTER 2
#define ENTER_HOSPITAL 3
#define ENTER_FANSHOP 4

#define ABORT 0

#define TIMESCALE 1

using namespace std;



bool isBidder;
bool hasChosen;

//TODO : affichage et récupération de l'input dans une seule méthode


void displayManagerInfos(int sockfd) {
  askForManagerInfos(sockfd);
  int nbPlayers, money, nbFans;
  receiveManagerInfos(sockfd,&nbPlayers,&money,&nbFans);
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<" -------------------- MONEY : "<<money<<" ||| "<<" NUMBER OF FANS : "<<nbFans<<" --------------------"<<endl;
}

void displayPlayersList(int sockfd) {
  askForPlayersList(sockfd);
  vector<string> playersList = receivePlayersList(sockfd);
  for (int i=0;i<playersList.size();i+=2) {cout<<" ["<<(i+2)/2<<"] "<<playersList[i]<<" "<<playersList[i+1]<<endl;}
}

void displayBuildingInfos(vector<int> buildingInfos, int buildingID){
  cout<<"Current level : "<<buildingInfos[0]<<endl;
  cout<<"Price for next level : "<<buildingInfos[1]<<endl;
  if (buildingID==STADIUM) cout<<"Max places in the Stadium : "<<buildingInfos[2]<<endl;
  else if (buildingID==TRAININGCENTER) cout<<"Time required to finish training : "<<buildingInfos[2]<<endl;
  else if (buildingID==HOSPITAL) cout<<"Time required to finish healing : "<<buildingInfos[2]<<endl;
  else if (buildingID==FANSHOP) cout<<"Max clients in the fan shop per match : "<<buildingInfos[2]<<endl;
  if (buildingInfos[3]==1) cout<<"This building is currently being upgraded"<<endl;
  else cout<<"This building is not upgrading yet.";
}

void displayPlayerInfos(int sockfd,vector<int> playerInfos, int playerID) {
  askForPlayersList(sockfd);
  vector<string> playersList = receivePlayersList(sockfd);
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

string displayAuctionInfos(vector<string> auctionsList,vector<int> playerInfos, int auctionID) {
  string name;
  string price;
  string timeLeft;
  for (int i=0;i<auctionsList.size();i+=4) {
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
    for (int i=0;i<auctionsList.size();i+=4) {
      cout<<" ["<<atoi(auctionsList[i].c_str())+1<<"] "<<auctionsList[i+1]<<" | Starting price : "<<auctionsList[i+3];
      cout<<" | Time left to participate : "<<auctionsList[2]<<endl;
    }
    cout<<"Indicate which auction you wish to inspect [or 0 to abort] : ";
  }
}

void displayConnexionMenu(){
  cout<<" ------------------------ WELCOME TO QUIDDITCH MANAGER 2014 ! ------------------------"<<endl;
  cout<<" [1] Log in"<<endl;;
  cout<<" [2] Create a new manager"<<endl;
  cout<<"-----> ";
}

void displayMainMenu(){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"What do you want to do ?"<<endl;
  cout<<" [1] See managers connected to propose match"<<endl;
  cout<<" [2] See/create auctions"<<endl;
  cout<<" [3] Manage players"<<endl;
  cout<<" [4] Manage buildings"<<endl;
  cout<<" [0] Quit game"<<endl;
  cout<<"-----> ";
}

void displayAuctionMenus(){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"//YOU'RE NOW IN THE AUCTION ROOM// What do you want to do ?"<<endl;
  cout<<" [1] See current auctions"<<endl;
  cout<<" [2] Sell a player"<<endl;
  cout<<" [0] Return to the main menu"<<endl;
  cout<<"-----> ";
}

void displaySellPlayerMenu(int sockfd){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"//YOU'RE ABOUT TO SELL A PLAYER// What do you want to do ?"<<endl;
  displayPlayersList(sockfd);
  cout<<" Which player do you want to sell [enter 0 to abort] ? "<<endl;
}

void displayManagePlayersMenu(){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"//MANAGE TEAM// What do you want to do ?"<<endl;
  cout<<" [1] Inspect a player"<<endl;
  cout<<" [2] Train a player"<<endl;
  cout<<" [3] Send a player to the hospital"<<endl;
  cout<<" [0] Return to the main menu"<<endl;
  cout<<"-----> ";
}

void displayManageBuildingsMenu(){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"//MANAGE BUILDINGS// What do you want to do ?"<<endl;
  cout<<" [1] Enter Stadium"<<endl;
  cout<<" [2] Enter Training Center"<<endl;
  cout<<" [3] Enter Hospital"<<endl;
  cout<<" [4] Enter FanShop"<<endl;
  cout<<" [0] Return to the main menu"<<endl;
  cout<<"-----> ";
}

void *auctionTurn(void* data) {

  int* sockfd = (int *)data;

  cout<<"Auction turn has started !"<<endl;
  cout<<" [1] Bid "<<endl;
  cout<<" [2] Quit auction"<<endl;


  int choice;
  cout<<" -----> ";
  cin>>choice;

  if (choice==1) {
    bid(*sockfd);
    int result = getConfirmation(*sockfd);
    isBidder=true;
    hasChosen=true;
  }
  else {
    isBidder=false;
    hasChosen=true;
  }

}


void mainAuction(int sockfd, int auctionID, int timeLeft) {

  hasChosen=false;
  isBidder=false;
  int turn = 0;

  pthread_t thread;
  int res = pthread_create(&thread, NULL, auctionTurn, (void*) &sockfd);
  sleep(timeLeft);
  pthread_cancel(thread);
  checkAuction(sockfd);
  int result = receiveAuctionResult(sockfd);
  int price = receiveAuctionResult(sockfd);
  if (result<0) {
    cout<<" -Congrats! You have won this auction"<<endl;
    if (result==-1) cout<<" --- "<<price<<" gold will be transferred in exchange of the player"<<endl;
    else if (result==-2) {
      cout<<" --- You don't have enough money to pay the "<<price<<" required";
      cout<<", so 3 percent of your money will be given to the owner of the player"<<endl;
    }
  }
  else if (result==0) {
    cout<<" -Goodbye! You have left this auction"<<endl;
  }
  else {
    cout<<" -Next turn will start soon... The current price is "<<price<<endl;
    sleep(10);
    turn+=1;

    while ((result>0)&&(hasChosen)&&(isBidder)) {
        hasChosen=false;
        res = pthread_create(&thread, NULL, auctionTurn, (void*)(&sockfd));
        sleep(30);
        pthread_cancel(thread);
        cout<<" --- END OF TURN"<<endl;
        checkAuction(sockfd);
        result = receiveAuctionResult(sockfd);
        price = receiveAuctionResult(sockfd);
        if ((!hasChosen)||(!isBidder)) {
          cout<<" -Goodbye! You have left this auction"<<endl;
        }
        else if (result<0) {
          cout<<" -Congrats! You have won this auction"<<endl;
          if (result==-1) cout<<" --- "<<price<<" gold will be transfered in exchange of the player"<<endl;
          else if (result==-2) {
            cout<<" --- You don't have enough money to pay the "<<price<<" required";
            cout<<", so 3 percent of your money will be given to the owner of the player"<<endl;
          }
        }
        else {
          cout<<" -Next turn will start soon... The current price is "<<price<<endl;
          sleep(10);
        }
        turn+=1;
      } 
    }
}












void startMatch(int sockfd){
  int winner = 0;
  int scoreTeam1 = 0;
  int scoreTeam2 = 0;
  HexagonalField field;
  std::vector<AxialCoordinates> allPositions;
  while(winner == 0){
    getAllPositions(sockfd);
    allPositions = receiveScoresAndPositions(sockfd, &winner, &scoreTeam1, &scoreTeam2);
    cout << "Score team1 (inviter) = " << scoreTeam1 << endl;
    cout << "Score team2 (invited) = " << scoreTeam2 << endl;
    field.reset();
    for(unsigned int i = 0; i < allPositions.size(); ++i){
      field.setOccupant(allPositions[i], i);
    }
    field.display();

    
    cout << "Les échanges de messages suivants pour le match n'ont pas encore été implémentés." << endl;
    winner =1;
  }
}
std::vector<int> displayAndAskPlayersForMatch(int sockfd){
  displayPlayersList(sockfd);
  std::cout << "Veuillez donner les index de vos joueur dans l'ordre suivant : KEEPER SEEKER CHASER1 CHASER2 CHASER3 BEATER1 BEATER2" << std::endl;
  int managedIndex;
  std::vector<int> playersInTeam;
  for(int i = 0; i < 7; ++i){
    cout << "indice joueur : " ;
    cin >> managedIndex;
    playersInTeam.push_back(managedIndex-1); //index commence à 0, affichage commence à 1
  }
  return playersInTeam;
}

void testMatchInvitation(int sockfd){
  isMatchWaiting(sockfd);
  if(getConfirmation(sockfd)){
    cout << "You've got a match proposal !" << endl;
    cout << "Accept ? [1/0]" << endl;
    bool confirmation;
    cin >> confirmation;
    std::vector<int> playersInTeam;
    if(confirmation){
      playersInTeam = displayAndAskPlayersForMatch(sockfd);
    }
    answerMatchProposal(sockfd, confirmation, playersInTeam);
    if(receiveMatchConfirmation(sockfd) == MATCH_STARTING){
      startMatch(sockfd);
    }
  }
}


    

int main(int argc, char *argv[]){
  int choice;
  bool result;
  int buildingID, userID;
  int confirm, price, moves[7][3];
  int att2;
  long att3;
  char att4[43];
  char username[USERNAME_LENGTH];
  char password[PASSWORD_LENGTH];
  char * position;
  SerializedObject toReceiveFromServer;
  int sockfd;
  struct sockaddr_in server_addr; 
  struct hostent *server;
  char serverName[DIM];
  
  if (argc != 2) {
    if(argc == 1){ /*pas d'arguments : serveur est sur la machine locale */
      gethostname(serverName, 60);
      printf("Le server est la machine locale, dont le nom est %s\n", serverName);
    }else{
      fprintf(stderr, "Donner le nom de la machine"
	    " distante en argument.");
	    return EXIT_FAILURE;
    }
    
  }else{
    int i = 0;
    while((argv[i] != '\0') && (i < DIM)){
      serverName[i] = argv[1][i];
      ++i;
    }
    serverName[i] = '\0';
	  printf("Le nom du server distant est %s\n", serverName);
  }
  

  if ((server=gethostbyname(serverName)) == NULL) { 
    perror("Client: gethostbyname");
    return EXIT_FAILURE;
  }
  
  
  if ((sockfd = socket(PF_INET, 
		       SOCK_STREAM, 0)) == -1) {
    perror("Client: socket");
    return EXIT_FAILURE;
  }
  
  server_addr.sin_family = AF_INET;  
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr = *((struct in_addr*)server->h_addr);
  memset(&(server_addr.sin_zero), '\0', 8); 
  
  if (connect(sockfd, (struct sockaddr *) &server_addr,
	      sizeof(struct sockaddr)) == -1) {
    perror("Client: connect");
    return 0;
  }
  
 /*
  displayConnexionMenu();
  scanf("%d",&choice);

  if (choice!=0) {
    printf("\nlogin : ");
    scanf("%s",username);
    printf("password : ");
    scanf("%s",password);

    if (choice==1) {
      sendLoginToServer(sockfd,username,password);
      getConfirmation(sockfd);
      askForManagerInfos(sockfd);
      int nbPlayers, money, nbFans;
      receiveManagerInfos(sockfd, &nbPlayers, &money, &nbFans);
      printf("nbPlayers money nbFans : %d, %d, %d", nbPlayers, money, nbFans);
      askForPlayersList(sockfd);

      getManagersList(sockfd);

      std::vector<std::string> list = receivePlayersList(sockfd);
      std::cout << list.size() << std::endl;
      for(unsigned int i = 0; i < list.size() ; ++i){
        std::cout << list[i] << std::endl;
      }
      askForPlayerInfos(sockfd, 0);
      std::vector<int> playerInfo = receivePlayerInfo(sockfd);
      for(unsigned int i = 0; i < playerInfo.size() ; ++i){
        std::cout << playerInfo[i] << std::endl;
      }
      askForBuildingInfos(sockfd, STADIUM);
      std::vector<int> buildingInfo = receiveBuildingInfos(sockfd);
      for(unsigned int i = 0; i < buildingInfo.size() ; ++i){
        std::cout << buildingInfo[i] << std::endl;
      }

      scanf("%s",username);
      printf("%s",username);
      answerMatchProposal(sockfd, 1, 1);
    }



    else if (choice==2) {
      sendNewManagerToServer(sockfd,username,password);
      proposeMatchTo(sockfd, 0);
    //result = serverAnswer
    }

  

  }
*/


  do {
    displayConnexionMenu();
  
    cin>>choice;
    cout<<"Enter login : ";
    cin>>username;
    cout<<"Enter password : ";
    cin>>password;

    if (choice==LOG_IN) sendLoginToServer(sockfd,username,password);
    else if (choice==NEW_MANAGER) sendNewManagerToServer(sockfd,username,password);
    result = getConfirmation(sockfd);
    if (!result) {
      if (choice==LOG_IN) cout<<" ------------- WRONG LOGIN/PASSWORD"<<endl;
      else if (choice==NEW_MANAGER) cout<<" ------------- LOGIN ALREADY TAKEN"<<endl;
    }
  } while (!result);

  do {
    testMatchInvitation(sockfd);
    displayManagerInfos(sockfd);
    displayMainMenu();
    cin>>choice;

    if (choice==SEE_MANAGERS) {
      testMatchInvitation(sockfd);
      getManagersList(sockfd);
      std::vector<int> IDList;
      std::vector<std::string> namesList;
      receiveManagersIDandNames(sockfd, &IDList,&namesList);
      for(unsigned int i = 0; i < IDList.size(); ++i){
        std::cout << "ID :" << IDList[i] << " name : " << namesList[i] << std::endl;
      }
      cout<<"Indicate the ID of the player you want to challenge : "; //TODO : retour en arrière, vérification des inputs, tester retours des send et receive
      int targetedUser;
      cin >> targetedUser;
      std::vector<int> playersInTeam = displayAndAskPlayersForMatch(sockfd);
      proposeMatchTo(sockfd, targetedUser,  playersInTeam);
      if(receiveMatchConfirmation(sockfd) == MATCH_STARTING){
        startMatch(sockfd);
      }
      
    }
    

    else if (choice==AUCTION_ROOM) {
      int auctionChoice;
      do {
        displayManagerInfos(sockfd);
        displayAuctionMenus();
        cin>>auctionChoice;
        if (auctionChoice==SEE_AUCTIONS) {
          getAuctionsList(sockfd);
          vector<string> auctionsList = receiveAuctionsList(sockfd);
          displayAuctionsList(auctionsList);
          if (auctionsList.size()!=0) {
            int auctionToInspect;
            cin>>auctionToInspect;
            if (auctionToInspect!=0) {
              askForAuctionInfos(sockfd,auctionToInspect-1);
              vector<int> playerAuctionInfos = receivePlayerInfo(sockfd);
              string timeLeft = displayAuctionInfos(auctionsList,playerAuctionInfos,auctionToInspect-1);
              cout<<"Do you want to join this auction ? [1 to enter, 0 to quit] \n -----> ";
              int enterAuction;
              cin>>enterAuction;
              if (enterAuction==1) {
                joinAuction(sockfd,auctionToInspect-1);
                int joinResult = getConfirmation(sockfd);
                mainAuction(sockfd,auctionToInspect-1,atoi(timeLeft.c_str()));
              }
            }
          }
        }
        else if (auctionChoice==SELL_PLAYER) {
          int sellPlayerChoice;
          do {
            displayManagerInfos(sockfd);
            displaySellPlayerMenu(sockfd);
            cin>>sellPlayerChoice;
            if (sellPlayerChoice!=ABORT) {
              askForPlayerInfos(sockfd,sellPlayerChoice-1);
              vector<int> playerInfos = receivePlayerInfo(sockfd);
              displayPlayerInfos(sockfd,playerInfos,sellPlayerChoice-1);
              cout<<"Indicate the starting price of the auction [or 0 to abort] \n -----> ";
              int startingPrice;
              cin>>startingPrice;
              if (startingPrice>0) {
                sellPlayer(sockfd,sellPlayerChoice-1,startingPrice);
                bool sellingResult = getConfirmation(sockfd); //always true
                if (!sellingResult) cout<<" ----- The player is blocked and cannot be selled right now"<<endl;
                else cout<<" ----- Auction started !"<<endl;
                sellPlayerChoice=ABORT;
              }
            }
          } while (sellPlayerChoice!=ABORT);
        }
    } while(auctionChoice!=ABORT);
  }

    else if (choice==MANAGE_PLAYERS) {
      testMatchInvitation(sockfd);
      int managePlayersChoice;
      do {
        displayManagerInfos(sockfd);
        displayManagePlayersMenu();
        cin>>managePlayersChoice;
        if (managePlayersChoice!=ABORT) {
          displayPlayersList(sockfd);
          int playerID;
          if (managePlayersChoice==INSPECT_PLAYER) {
            cout<<"Indicate the number of the player you wish to inspect [or 0 to abort] \n -----> ";
            cin>>playerID;
            if (playerID>0) {
              askForPlayerInfos(sockfd,playerID-1);
              vector<int> playerInfos = receivePlayerInfo(sockfd);
              displayPlayerInfos(sockfd,playerInfos,playerID-1);
            }
          }
          else if (managePlayersChoice==TRAIN_PLAYER_OPTION) {
            int capacityNumber;
            cout<<"Indicate the number of the player you wish to train  \n -----> ";
            cin>>playerID;
            cout<<"Indicate the number of the capacity you wish to train [or 0 to abort]  \n -----> ";
            cin>>capacityNumber;
            if ((playerID>0)&&(capacityNumber>0)) {
              trainPlayer(sockfd,playerID-1,capacityNumber-1);
              bool trainingResult = getConfirmation(sockfd);
              if (trainingResult) {
                askForBuildingInfos(sockfd,TRAININGCENTER);
                vector<int> trainingCenterInfos = receiveBuildingInfos(sockfd);
                cout<<"-------------\n Training has started !"<<endl;
                cout<<" Training will be over in "<<trainingCenterInfos[2]<<" minutes"<<endl;
              }
              else cout<<"------------\n Training impossible, this player is blocked"<<endl;
            }
          }

          else if (managePlayersChoice==HEAL_PLAYER_OPTION) {
            cout<<"Indicate the number of the player you wish to heal [or 0 to abort] : ";
            cin>>playerID;
            if (playerID!=ABORT) {
              healPlayer(sockfd,playerID-1);
              int healResult = getConfirmation(sockfd);
              if (healResult){
                askForBuildingInfos(sockfd,HOSPITAL);
                vector<int> hospitalInfos = receiveBuildingInfos(sockfd);
                cout<<"-------------\n This player has been sent to the hospital !"<<endl;
                cout<<" Player will leave the hospital in "<<hospitalInfos[2]<<" minutes"<<endl;
              }
              else cout<<"-------------\n This player cannot be send to the hospital, he's blocked or in full health"<<endl;
            }
          }
        }
      } while (managePlayersChoice!=ABORT);
    }
    else if (choice==MANAGE_BUILDINGS) {
      testMatchInvitation(sockfd);
      int manageBuildingsChoice;
      do {
        displayManagerInfos(sockfd);
        displayManageBuildingsMenu();
        cin>>manageBuildingsChoice;
        if (manageBuildingsChoice!=ABORT) {
          askForBuildingInfos(sockfd,manageBuildingsChoice);
          vector<int> buildingInfos = receiveBuildingInfos(sockfd);
          displayBuildingInfos(buildingInfos,manageBuildingsChoice);


          cout<<"\n---------------\nEnter 1 if you wish to upgrade this building [or 0 to abort] \n -----> ";

          //TODO : tester niveau client si argent suffisant
          int upgrade;
          cin>>upgrade;
          if (upgrade) {
            askForBuildingUpgrade(sockfd,manageBuildingsChoice);
            int upgradeResult = getConfirmation(sockfd);
            if (upgradeResult) {
              cout<<"-------------\n This building has started upgrading !"<<endl;
              cout<<" Upgrade will be complete in "<<(1+buildingInfos[0])*TIMESCALE<<" minutes !"<<endl;
            }
            else cout<<"-------------\n You can't upgrade this building (not enough money or this building is already upgrading) !"<<endl;
          }
        }
      } while (manageBuildingsChoice!=ABORT);
    }
  } while (choice!=ABORT);
  
  return EXIT_SUCCESS;
} 
